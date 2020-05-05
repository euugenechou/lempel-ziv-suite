# A Suite of Lempel-Ziv Algorithms

This repo is comprised of implementations for the two most popular Lempel-Ziv
algorithms: LZ78 and LZW.

### Design

Each implementation is comprised of two programs, ```encode``` and ```decode```,
which perform compression and decompression, respectively. Each implementation
utilizes variable code bitwidths, where the maximum bitwidth for a code is 16.
It is, however, very easy to change the maximum bitwidth by going into each
implementation's ```util.h``` file and changing the value of the ```MAX```
macro.

### Building

To build both the encoder and decoder for each implementation:

    make all
    make

To build just the encoder:

    make encode

To build just the decoder:

    make decode

### Usage

To compress a file (if input or output isn't specified, ```stdin``` and
```stdout``` are used by default, respectively):

    $ ./encode [-i input] [-o output]

To decompress a file (if input or output isn't specified, ```stdin``` and
```stdout``` are used by default, respectively):

    $ ./decode [-i input] [-o output]


### Additional Program Options

Both the encoder and decoder support the command-line options below.

    '-h': Display program usage and help
    '-v': Display statistics on compression/decompression
