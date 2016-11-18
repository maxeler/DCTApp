# DCT

## Description

This application accelerates discrete cosine transform (DCT) using Maxeler dataflow engines (DFEs). 

DCTs are widely used in modern signal and image processing applications, in particular for implementing lossy compression of audio, image and video data. 

A DCT expresses a finite sequence of discrete data points in terms of sums of basis cosine functions oscillating at different frequencies. The 2D DCT-II alternative, which underlies JPEG compression, splits an image into 8`x`8 pixel tiles and encodes each tile independently. The algorithm reduces the entropy of each tile by discarding points or sequences of bits that are redundant or carry less important signal.

More details can be found in the DOCS folder.


## Content

The repository root directory contains the following items:

- APP
- DOCS
- INPUT
- maxpower
- LICENCE.txt

### APP

Directory containing project sources.

### DOCS

Documentation of the project.

### INPUT

Directory containing a sample input file for an image of size 128`x`128.

### maxpower

Directory containing a copy of [Maxeler standard library (MaxPower)](https://github.com/maxeler/maxpower "MaxPower")

### LICENSE.txt

License of the project.

## How to build

Make sure the following environment variables are set correctly:
  * `MAXELEROSDIR`
  * `MAXCOMPILERDIR`

The application uses Maxeler's standard library maxpower which is included as a git submodule. To clone maxpower, run the following commands:

	git submodule init
	git submodule update

### Compiling MaxJ code

This is a pre-requisite step to compiling the application.

	- open the MaxIDE,
	- import the APP directory as a MaxCompiler project,
	- import the maxpower directory as non-MaxCompiler project,
	- build both maxpower and DCT projects,
	- make sure APP/EngineCode/bin and maxpower/bin directores have been created.

### Compiling the bitstream and the CPU binary

To compile the application, go to ./APP/CPUCode/ and run:

    make RUNRULE="<configuration>"

where currently `<configuration>` can be either Simulation or DFE.

To delete the distributed maxfiles before recompiling the application, run the following command before compilation:

    make RUNRULE="<configuration>" distclean

### Makefile targets

#### build  

Compiles the application

#### clean  

Deletes from build directories files produced during compilation 

#### distclean  

Deletes from build directories all files produced during configuration and compilations, including all maxfiles

## Usage

The DCT application operates on square image matrices of size N`x`N where N is a multiple of 8 and each element of the matrix is a number in the range [0, 255].  The matrix can be either input from a file or generated by the application automatically at random.

### Reading image input data from files

A sample image of size N`x`N is provided under the ./INPUT directory.  The file format takes one pixel of data per line and pixels are listed in a row-major order. Example:

	./APP/RunRules/Simulation/binaries/DCT -i ./INPUT/input_128x128.dsp

### Using randomly generated images

You can process an image of size N`x`N by providing the value of N on the command line. Example:

	./APP/RunRules/Simulation/binaries/DCT -r 1024

### Running application in simulation from command line

To start the simulator run

	maxcompilersim -d 1 -c MAIA -n dctapp start

Export necessary environmental variables

	export MAXELEROSDIR=$MAXCOMPILERDIR/lib/maxeleros-sim
    export LD_PRELOAD=$MAXELEROSDIR/lib/libmaxeleros.so:$LD_PRELOAD
    export SLIC_CONF="$SLIC_CONF;use_simulation=dctapp"

Run the binary (see above for explanations), e.g.

	./APP/RunRules/Simulation/binaries/DCT -i ./INPUT/input_128x128.dsp

Stop the simulator

	maxcompilersim -d 1 -c MAIA -n dctapp stop
