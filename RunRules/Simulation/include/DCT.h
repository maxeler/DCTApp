/**\file */
#ifndef SLIC_DECLARATIONS_DCT_H
#define SLIC_DECLARATIONS_DCT_H
#include "MaxSLiCInterface.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DCT_PCIE_ALIGNMENT (16)


/*----------------------------------------------------------------------------*/
/*---------------------------- Interface default -----------------------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'default'.
 * 
 * \param [in] ticks_DCTInt8Kernel The number of ticks for which kernel "DCTInt8Kernel" will run.
 * \param [in] instream_Input Stream "Input".
 * \param [in] instream_size_Input The size of the stream instream_Input in bytes.
 * \param [in] instream_cos Stream "cos".
 * \param [in] instream_size_cos The size of the stream instream_cos in bytes.
 * \param [out] outstream_block_out_HW Stream "block_out_HW".
 * \param [in] outstream_size_block_out_HW The size of the stream outstream_block_out_HW in bytes.
 * \param [in] inmem_DCTInt8Kernel_romDivBy2 Mapped ROM inmem_DCTInt8Kernel_romDivBy2, should be of size (512 * sizeof(uint64_t)).
 * \param [in] inmem_DCTInt8Kernel_romDivBy3 Mapped ROM inmem_DCTInt8Kernel_romDivBy3, should be of size (512 * sizeof(uint64_t)).
 */
void DCT(
	uint64_t ticks_DCTInt8Kernel,
	const void *instream_Input,
	size_t instream_size_Input,
	const void *instream_cos,
	size_t instream_size_cos,
	void *outstream_block_out_HW,
	size_t outstream_size_block_out_HW,
	const uint64_t *inmem_DCTInt8Kernel_romDivBy2,
	const uint64_t *inmem_DCTInt8Kernel_romDivBy3);

/**
 * \brief Basic static non-blocking function for the interface 'default'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] ticks_DCTInt8Kernel The number of ticks for which kernel "DCTInt8Kernel" will run.
 * \param [in] instream_Input Stream "Input".
 * \param [in] instream_size_Input The size of the stream instream_Input in bytes.
 * \param [in] instream_cos Stream "cos".
 * \param [in] instream_size_cos The size of the stream instream_cos in bytes.
 * \param [out] outstream_block_out_HW Stream "block_out_HW".
 * \param [in] outstream_size_block_out_HW The size of the stream outstream_block_out_HW in bytes.
 * \param [in] inmem_DCTInt8Kernel_romDivBy2 Mapped ROM inmem_DCTInt8Kernel_romDivBy2, should be of size (512 * sizeof(uint64_t)).
 * \param [in] inmem_DCTInt8Kernel_romDivBy3 Mapped ROM inmem_DCTInt8Kernel_romDivBy3, should be of size (512 * sizeof(uint64_t)).
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *DCT_nonblock(
	uint64_t ticks_DCTInt8Kernel,
	const void *instream_Input,
	size_t instream_size_Input,
	const void *instream_cos,
	size_t instream_size_cos,
	void *outstream_block_out_HW,
	size_t outstream_size_block_out_HW,
	const uint64_t *inmem_DCTInt8Kernel_romDivBy2,
	const uint64_t *inmem_DCTInt8Kernel_romDivBy3);

/**
 * \brief Advanced static interface, structure for the engine interface 'default'
 * 
 */
typedef struct { 
	uint64_t ticks_DCTInt8Kernel; /**<  [in] The number of ticks for which kernel "DCTInt8Kernel" will run. */
	const void *instream_Input; /**<  [in] Stream "Input". */
	size_t instream_size_Input; /**<  [in] The size of the stream instream_Input in bytes. */
	const void *instream_cos; /**<  [in] Stream "cos". */
	size_t instream_size_cos; /**<  [in] The size of the stream instream_cos in bytes. */
	void *outstream_block_out_HW; /**<  [out] Stream "block_out_HW". */
	size_t outstream_size_block_out_HW; /**<  [in] The size of the stream outstream_block_out_HW in bytes. */
	const uint64_t *inmem_DCTInt8Kernel_romDivBy2; /**<  [in] Mapped ROM inmem_DCTInt8Kernel_romDivBy2, should be of size (512 * sizeof(uint64_t)). */
	const uint64_t *inmem_DCTInt8Kernel_romDivBy3; /**<  [in] Mapped ROM inmem_DCTInt8Kernel_romDivBy3, should be of size (512 * sizeof(uint64_t)). */
} DCT_actions_t;

/**
 * \brief Advanced static function for the interface 'default'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void DCT_run(
	max_engine_t *engine,
	DCT_actions_t *interface_actions);

/**
 * \brief Advanced static non-blocking function for the interface 'default'.
 *
 * Schedule the actions to run on the engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in] interface_actions Actions to be executed.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *DCT_run_nonblock(
	max_engine_t *engine,
	DCT_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'default'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void DCT_run_group(max_group_t *group, DCT_actions_t *interface_actions);

/**
 * \brief Group run advanced static non-blocking function for the interface 'default'.
 * 
 *
 * Schedule the actions to run on the first device available in the group and return immediately.
 * The status of the run must be checked with ::max_wait. 
 * Note that use of ::max_nowait is prohibited with non-blocking running on groups:
 * see the ::max_run_group_nonblock documentation for more explanation.
 *
 * \param [in] group Group to use.
 * \param [in] interface_actions Actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *DCT_run_group_nonblock(max_group_t *group, DCT_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'default'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void DCT_run_array(max_engarray_t *engarray, DCT_actions_t *interface_actions[]);

/**
 * \brief Array run advanced static non-blocking function for the interface 'default'.
 * 
 *
 * Schedule to run the array of actions on the array of engines, and return immediately.
 * The length of interface_actions must match the size of engarray.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * \param [in] engarray The array of devices to use.
 * \param [in] interface_actions The array of actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *DCT_run_array_nonblock(max_engarray_t *engarray, DCT_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* DCT_convert(max_file_t *maxfile, DCT_actions_t *interface_actions);

/**
 * \brief Initialise a maxfile.
 */
max_file_t* DCT_init(void);

/* Error handling functions */
int DCT_has_errors(void);
const char* DCT_get_errors(void);
void DCT_clear_errors(void);
/* Free statically allocated maxfile data */
void DCT_free(void);
/* returns: -1 = error running command; 0 = no error reported */
int DCT_simulator_start(void);
/* returns: -1 = error running command; 0 = no error reported */
int DCT_simulator_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SLIC_DECLARATIONS_DCT_H */

