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

## Usage

Once coal is built, you just have to run `coal <seq1.fasta> <seq2.fasta>` to two sequences.

## Possible improvements
