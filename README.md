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

    $ make all

Or

    $ make

To build just the encoder:

    $ make encode

To build just the decoder:

    $ make decode

### Usage

To compress a file (```stdin``` and ```stdout``` are the default input and
output, respectively):

    $ ./encode [-i input] [-o output]

To decompress a file (```stdin``` and ```stdout``` are the default input and
output, respectively):

    $ ./decode [-i input] [-o output]


### Additional Program Options

Both the encoder and decoder support the command-line options below.

    '-h': Display program usage and help
    '-v': Display statistics on compression/decompression

### Benchmarks

A ```python3``` script, ```benchmark.py``` has been supplied for easy comparison
between each of the Lempel-Ziv implementations. Both the UNIX ```compress``` and
```gzip``` utilities are included for comparison as well. To use the script:

    $ python3 benchmark.py [--artificial] [--calgary] [--canterbury] [--large]

Or if the supplied shebang/hashbang points to the correct interpreter:

    $ ./benchmark.py [--artificial] [--calgary] [--canterbury] [--large]

The texts tested with each implemenation are part of a corpora, or more
specifically, the Artificial Corpus, Calgary Corpus, Canterbury Corpus, and
Large Corpus, as supplied from http://corpus.canterbury.ac.nz/index.html. The
corpora is widely used for testing lossless compression algorithms. The script
also allows for an ```--all``` flag to test with the supplied corpora.

## License

Copyright 2020 Eugene Chou

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

For him that stealeth this code and name it as his own let it change to a
serpent in his hand and rend him. Let him be struck with palsy, and
all his members wither. Let him languish in pain, crying aloud for
mercy and let there be no surcease to his agony till he sink to
dissolution. Let worms gnaw his entrails in token of the worm
that dieth not, and when at last he goeth to his final punishment
let the flames of Hell consume him.
