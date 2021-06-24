FLAGS := -O3 -g -Wall -Wextra -Wpedantic
FLAGS_AVX := -march=native
COMMON := fasta.c utils.c
REPEAT := 1000

BUILD ?= build
ALIGN_STANDARD := ${BUILD}/align_standard
ALIGN_HASH := ${BUILD}/align_hash
ALIGN_VECT := ${BUILD}/align_vect

all: ${ALIGN_STANDARD} ${ALIGN_HASH} ${ALIGN_VECT}

${ALIGN_STANDARD}: align_standard.c nw_diag.c ${COMMON}
	mkdir -p ${BUILD}
	gcc ${FLAGS} $^ -o $@
	gcc ${FLAGS} -DTRACEBACK $^ -o $@_tb
	gcc ${FLAGS} -DTRACEBACK -DSHOW_SEQ $^ -o $@_show_tb
	gcc ${FLAGS} -DREPEAT=${REPEAT} $^ -o $@_${REPEAT}

${ALIGN_HASH}: align_hash.c hash.c nw_diag.c ${COMMON}
	mkdir -p ${BUILD}
	gcc ${FLAGS} $^ -o $@
	gcc ${FLAGS} -DTRACEBACK $^ -o $@_tb
	gcc ${FLAGS} -DTRACEBACK -DSHOW_SEQ $^ -o $@_show_tb
	gcc ${FLAGS} -DREPEAT=${REPEAT} $^ -o $@_${REPEAT}
	gcc ${FLAGS} -DBORDERLESS $^ -o $@_b
	gcc ${FLAGS} -DBORDERLESS -DREPEAT=${REPEAT} $^ -o $@_b_${REPEAT}

${ALIGN_VECT}: align_vect.c hash.c nw_diag.c nw_vect.c ${COMMON}
	mkdir -p ${BUILD}
	gcc ${FLAGS} ${FLAGS_AVX} $^ -o $@
	gcc ${FLAGS} ${FLAGS_AVX} -DTRACEBACK $^ -o $@_tb
	gcc ${FLAGS} ${FLAGS_AVX} -DTRACEBACK -DSHOW_SEQ $^ -o $@_show_tb
	gcc ${FLAGS} ${FLAGS_AVX} -DREPEAT=${REPEAT} $^ -o $@_${REPEAT}
	gcc ${FLAGS} ${FLAGS_AVX} -DBORDERLESS $^ -o $@_b
	gcc ${FLAGS} ${FLAGS_AVX} -DBORDERLESS -DREPEAT=${REPEAT} $^ -o $@_b_${REPEAT}

clean:
	$(RM) -r ${BUILD}
