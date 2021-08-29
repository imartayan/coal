CC := gcc
FLAGS := -O3 -g -Wall -Wextra -Wpedantic
FLAGS_AVX := -march=native

BUILD ?= build
COAL := ${BUILD}/coal
COAL_VECT := ${BUILD}/coal_vect
COAL_STD := ${BUILD}/coal_standard
COAL_BENCHMARK := ${BUILD}/coal_benchmark
CUT := ${BUILD}/cut
REPEAT := 10

.PHONY: all vect benchmark clean

all: ${COAL_STD} ${COAL} ${CUT}

vect: ${COAL_STD} ${COAL_VECT} ${COAL_BENCHMARK} ${CUT}

COMMON := fasta.c width.c nw.c

${COAL_STD}: coal_standard.c ${COMMON}
	@ mkdir -p ${BUILD}
	${CC} ${FLAGS} -DTRACEBACK -DSHOW_SEQ $^ -o $@

${COAL}: coal.c hash.c ${COMMON}
	@ mkdir -p ${BUILD}
	${CC} ${FLAGS} -DTRACEBACK -DSHOW_SEQ $^ -o $@

${COAL_VECT}: coal_vect.c hash.c nw_vect.c ${COMMON}
	@ mkdir -p ${BUILD}
	${CC} ${FLAGS} ${FLAGS_AVX} -DTRACEBACK -DSHOW_SEQ $^ -o $@

${COAL_BENCHMARK}: coal_vect.c hash.c nw_vect.c ${COMMON}
	@ mkdir -p ${BUILD}
	${CC} ${FLAGS} ${FLAGS_AVX} -DREPEAT=${REPEAT} $^ -o $@_${REPEAT}

${CUT}: cut.c hash.c fasta.c
	@ mkdir -p ${BUILD}
	${CC} ${FLAGS} $^ -o $@

# Example: changing kmer size for fragmentation
${COAL}_20: coal.c hash.c nw_diag.c ${COMMON}
	@ mkdir -p ${BUILD}
	${CC} ${FLAGS} -DKMER_SIZE=20 -DTRACEBACK -DSHOW_SEQ $^ -o $@

clean:
	$(RM) -r ${BUILD}
