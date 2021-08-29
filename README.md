# Coal - consensus alignement

Coal is a sequence alignment tool for long similar reads (produced from the same region).
It was developed during my internship in the [GenScale team](https://team.inria.fr/genscale/) at [Inria Rennes](https://www.inria.fr/en/centre-inria-rennes-bretagne-atlantique).

## Project structure

* `nw.c` contains a banded version of Needleman-Wunsch algorithm
* `nw_vect.c` contains a vectorized implementation of the same algorithm
* `width.c` defines heuristics used for choosing band width
* `hash.c` contains a simple implementation of a hash table used for storing kmers
* `fasta.c` contains a simple parser for fasta files
* `coal.c` contains the implementation of the fragmented alignment method
* `coal_vect.c` contains a vectorized version of the same method
* `coal_standard.c` contains a standard alignment method without fragmentation (used mainly for comparisons)
* `cut.c` contains a tool for sequence preprocessing

## Building

This project has no dependencies, so you just have to use `make` to build the executables.

If your processor supports the [AVX2 instruction set](https://software.intel.com/sites/landingpage/IntrinsicsGuide/), it is recommended to use the vectorized version for better performances : just type `make vect`.
Also note that you need have `gcc` in order to compile AVX instructions (on macOS make sure that the `CC` variable in the Makefile refers to the actual `gcc` compiler - not `clang`).

If you want to change specific parameters of the algorithm such as the size of kmers, have a look at the Makefile to see the different flags you can set for compilation.

## Usage

Once coal is built, you just have to run `./coal <seq1.fasta> <seq2.fasta>` to align two sequences.
The same applies to `coal_vect` and `coal_standard`.

You can find some test sequences in the `examples` directory.

This repository also provides a tool named `cut` which can be used to preprocess a pair of sequences :
`./cut <seq1.fasta> <seq2.fasta> <out1> <out2>`.
What `cut` does is that it cuts (sic) the given sequences between their first and their last anchor points.
This might be useful if the sequences have an important offset.

## Possible improvements

* provide a complete CLI instead of obscure compilation flags
* provide a vectorized version of Gotoh's algorithm as an alternative to Needleman-Wunsch
* make it easier to use an external library for the alignment of subsequences (instead of `nw.h`)
* improve the hashing method (e.g. by using Robin Hood Hashing or Cuckoo Hashing)
* extend vectorization for bigger band width (32, 64, or even an arbitrary width)
* refine the choice of kmers used as anchor points
