FLAGS := -O3 -g -Wall -Wextra -Wpedantic
FLAGS_AVX := -march=native
COMMON := fasta.c utils.c

BUILD ?= build
ALIGN_STANDARD := ${BUILD}/align_standard
ALIGN_HASH := ${BUILD}/align_hash
ALIGN_VECT := ${BUILD}/align_vect

all: ${ALIGN_STANDARD} ${ALIGN_HASH} ${ALIGN_VECT}

${ALIGN_STANDARD}: align_standard.c nw_diag.c fasta.c ${COMMON}
	mkdir -p ${BUILD}
	gcc ${FLAGS} $^ -o $@
	gcc ${FLAGS} -DTRACEBACK $^ -o $@_tb
	gcc ${FLAGS} -DTRACEBACK -DSHOW_SEQ $^ -o $@_show_tb
	gcc ${FLAGS} -DREPEAT=100 $^ -o $@_100

${ALIGN_HASH}: align_hash.c hash.c nw_diag.c ${COMMON}
	mkdir -p ${BUILD}
	gcc ${FLAGS} $^ -o $@
	gcc ${FLAGS} -DTRACEBACK $^ -o $@_tb
	gcc ${FLAGS} -DTRACEBACK -DSHOW_SEQ $^ -o $@_show_tb
	gcc ${FLAGS} -DREPEAT=100 $^ -o $@_100

${ALIGN_VECT}: align_vect.c hash.c nw_diag.c nw_vect.c ${COMMON}
	mkdir -p ${BUILD}
	gcc ${FLAGS} ${FLAGS_AVX} $^ -o $@
	gcc ${FLAGS} ${FLAGS_AVX} -DTRACEBACK $^ -o $@_tb
	gcc ${FLAGS} ${FLAGS_AVX} -DTRACEBACK -DSHOW_SEQ $^ -o $@_show_tb
	gcc ${FLAGS} ${FLAGS_AVX} -DREPEAT=100 $^ -o $@_100

clean:
	$(RM) -r ${BUILD}
