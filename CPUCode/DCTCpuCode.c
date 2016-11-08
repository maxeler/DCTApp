#include <math.h>
#include <stdio.h>
#include "stdint.h"
#include <stdlib.h>
#include <sys/time.h>
#include "Maxfiles.h"
#include "MaxSLiCInterface.h"
#define         N               128
#define         B               8

#define         ROUND(f)        (((f)<0.0) ? (int)((f)-0.5) : (int)((f)+0.5))

#define FIXED_POINT 16
#define ONE  1 << FIXED_POINT

unsigned long getTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}

int fromFloatToFixedPoint(float val) {
	return (int) (val * (ONE));
}

/* Compute the SW dct function */
void dct_SW(int block_SW[B * B], float temp2d[B * B], float cos1[B * B],
		float cos2[B * B], int res[B * B]) {
	int i;
	int j;
	int k;
	float sum;
	for (i = 0; i < B; i++) {
		for (j = 0; j < B; j++) {
			sum = 0.0;
			for (k = 0; k < B; k++) {
				sum += block_SW[B * i + k] * cos2[B * k + j];
			}
			temp2d[B * i + j] = sum;
		}
	}

	for (i = 0; i < B; i++) {
		for (j = 0; j < B; j++) {
			sum = 0.0;
			for (k = 0; k < B; k++) {
				sum += cos1[B * i + k] * temp2d[B * k + j];
			}
			res[B * i + j] = ROUND(sum);
		}
	}
}

/* Pack 2 integers into 8 bits */
int pack2in8(int a, int b) {
	/* limit to signed 4 bits */
	a &= 0x0f;
	b &= 0x0f;
	return ((a << 4) | (b));
}

/* Pack 4 integers into 32 bits */
int pack4in32(int a, int b, int c, int d) {
	/* limit to signed 8 bits */
	a &= 0xff;
	b &= 0xff;
	c &= 0xff;
	d &= 0xff;
	return ((a << 24) | (b << 16) | (c << 8) | (d));
}

/* Pack the dct SW result in order to obtain a data compression */
void Pack_Data(int index, int vect_in[], int vect_out[]) {

	vect_out[index] = pack4in32(vect_in[0], vect_in[1], pack2in8(
			vect_in[2] / 3, vect_in[3] / 3), pack2in8(vect_in[4] / 2,
			vect_in[5] / 2));

	vect_out[index + 1] = pack4in32(pack2in8(vect_in[6], vect_in[7]),
			vect_in[8], vect_in[9], pack2in8(vect_in[10] / 2, vect_in[11]));

	vect_out[index + 2] = pack4in32(pack2in8(vect_in[12], vect_in[13]),
			pack2in8(vect_in[16] / 3, vect_in[17] / 2), pack2in8(vect_in[18]
					/ 2, vect_in[19]), pack2in8(vect_in[24] / 3, vect_in[25]));

	vect_out[index + 3] = pack4in32(pack2in8(vect_in[26], vect_in[27]),
			pack2in8(vect_in[32] / 2, vect_in[33]), pack2in8(vect_in[40] / 2,
					vect_in[41]), pack2in8(vect_in[48], vect_in[56]));
}

int Check_Data(int SW_Results[], int HW_Results[]) {
	int failures = 0;
	int dim = (N / B) * (N / B) * 4;
	for (int i = 0; i < dim; i++) {
		if (abs(SW_Results[i] - HW_Results[i]) >= 1) {
			printf("*** Error for index %d - SW: %d, HW: %d\n", i,
					SW_Results[i], HW_Results[i]);
			failures++;
		}
	}
	return failures;
}

int main(void) {
	float factor1;
	float factor2;
	float temp_cos;

	double tSW0 = 0, tSW1 = 0, tHW0 = 0, tHW1 = 0, tSW = 0, tHW = 0;

	int sizeBlock = B * B;
	int num_blocks = N / B;
	int *Input_SW_image = calloc(N * N, sizeof(int));
	int *Input_block_SW = calloc(sizeBlock, sizeof(int));
	float *cos1 = calloc(sizeBlock, sizeof(float));
	float *cos2 = calloc(sizeBlock, sizeof(float));
	float *temp2d_SW = calloc(sizeBlock, sizeof(float));
	int *Output_block_SW = calloc(sizeBlock, sizeof(int));

	int indexHW = 0;
	uint8_t *Input_HW_image = calloc(N * N, sizeof(uint8_t));
	int *cos2Fixed_32 = calloc(sizeBlock, sizeof(int));

	int index_Pack = 0;
	int final_dim = (N / B) * (N / B) * 4;
	int *HW_Results = calloc(final_dim, sizeof(int));
	int *SW_Results = calloc(final_dim, sizeof(int));

	size_t sizeBytesInt = sizeBlock * sizeof(int);
	int64_t *romDivBy2 = calloc(512, sizeof(int64_t));
	int64_t *romDivBy3 = calloc(512, sizeof(int64_t));

	int k;
	int l;
	int m;
	int n;
	int i = 0;
	int failures = 0;

	printf("*********** DISCRETE COSINE TRANSFORMATION ***********\n");
	printf("Initialization of the input data...\n");

	/* Initialize the cosine matrices ("cos2" is the transpose of "cos1") */
	factor1 = 2.0 * atan(1.0) / B;
	factor2 = 0.0;

	for (m = 0; m < B; ++m) {
		for (n = 0; n < B; ++n) {
			temp_cos = cos(factor2 * (2 * n + 1)) / B;
			cos1[B * m + n] = temp_cos;
			cos2[B * n + m] = temp_cos;
		}
		factor2 += factor1;
	}

	/* Open the file of the input data */
	FILE *fd;
	char buf[10];
	char *res;
	fd = fopen("input_16384x16384.dsp", "r");
	//fd = fopen("input_128x128.dsp", "r");
	if (fd == NULL) {
		perror("Error during the opening of the input file!");
		return (1);
	}
	printf("Done!\n");
	printf("Read the input image...\n");
	while (1) {
		res = fgets(buf, 10, fd);
		if (res == NULL)
			break;
		Input_SW_image[i] = atoi(res);
		if (i == N * N - 1)
			break;
		i++;
	}
	fclose(fd);
	printf("Done!\n");

	/*Compute the SW results */

	printf("Compute the SW results...\n");
	for (m = 0; m < num_blocks; m++) {
		for (n = 0; n < num_blocks; n++) {
			// Read next image block.
			for (k = 0; k < B; k++) {
				for (l = 0; l < B; l++) {
					Input_block_SW[B * k + l] = Input_SW_image[B * (B * m + k)
							+ B * n + l];
					// Create a linearized version of the input matrix for the HW computation
					Input_HW_image[indexHW] = Input_SW_image[B * (B * m + k)
							+ B * n + l];
					indexHW++;
				}
			}

			tSW0 = getTime();
			for (i = 0; i < 100; i++) {
				dct_SW(Input_block_SW, temp2d_SW, cos1, cos2, Output_block_SW);
				Pack_Data(index_Pack, Output_block_SW, SW_Results);
			}
			tSW1 = getTime();
			tSW += tSW1 - tSW0;

			index_Pack = index_Pack + 4;
		}
	}

	printf("Done!\n");

	//sleep(5)


	/* Generate the ROM Content */
	for (i = 0; i < 512; i++) {
		romDivBy2[i] = (i - 255) / 2;
		romDivBy3[i] = (i - 255) / 3;
	}

	/* Convert the cos2 matrix from floating to fixed point */
	for (m = 0; m < B * B; ++m) {
		cos2Fixed_32[m] = fromFloatToFixedPoint(cos2[m]);
	}

	printf("Compute the HW results...\n");
	max_file_t *maxfile = DCT_init();
	max_engine_t *engine = max_load(maxfile, "*");
	max_actions_t* act = max_actions_init(maxfile, NULL);
	max_queue_input(act, "cos", cos2Fixed_32, sizeBytesInt);
	max_queue_input(act, "Input", Input_HW_image, N * N * sizeof(int8_t));
	max_set_mem_range_uint64t(act, "DCTInt8Kernel", "romDivBy2", 0, 512,
			romDivBy2);
	max_set_mem_range_uint64t(act, "DCTInt8Kernel", "romDivBy3", 0, 512,
			romDivBy3);
	max_queue_output(act, "block_out_HW", HW_Results, final_dim * sizeof(int));
	max_set_ticks(act, "DCTInt8Kernel", num_blocks * num_blocks);

	tHW0 = getTime();
	for (i = 0; i < 100; i++)
		max_run(engine, act);
	tHW1 = getTime();
	tHW = tHW1 - tHW0;

	max_unload(engine);

	printf("Done!\n");

	/*Check the correctness of the results*/
	failures = Check_Data(SW_Results, HW_Results);
	/* Write of the results into a file*/
	FILE *fd_out;
	//fd_out = fopen("output_16384x16384.dsp", "w");
	fd_out = fopen("output_128x128.dsp", "w");
	if (fd_out == NULL) {
		perror("Error during the opening of the output file!");
		return (1);
	}
	printf("Done!\n");
	for(i=0;i<final_dim;i=i+4)
		fprintf(fd_out, "%d %d %d %d\n", HW_Results[i], HW_Results[i+1], HW_Results[i+2], HW_Results[i+3]);
	fclose(fd_out);
	printf("Done!\n");

	printf("SW time: %lf seconds\n", tSW / 1000000);
	printf("HW time: %lf seconds\n", tHW / 1000000);
	double corr_res = (((double) (final_dim - failures) * 100) / (final_dim));
	double err = (double) (failures * 100) / (final_dim);
	printf("TOTAL CORRECT RESULTS: %d/%d (%lf %%)\n", (final_dim - failures),
			final_dim, corr_res);
	printf("TOTAL ERRORS: %d/%d (%lf %%)\n", failures, final_dim, err);

	printf("*********** 	   DONE		   ***********\n");
	return 0;

}
