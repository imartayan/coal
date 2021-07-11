CC := gcc
FLAGS := -O3 -g -Wall -Wextra -Wpedantic
FLAGS_AVX := -march=native
COMMON := fasta.c width.c

BUILD ?= build
CUT := ${BUILD}/cut
ALIGN_STANDARD := ${BUILD}/align_standard
ALIGN_HASH := ${BUILD}/align_hash
ALIGN_VECT := ${BUILD}/align_vect

REPEAT := 10

all: ${CUT} ${ALIGN_STANDARD} ${ALIGN_HASH} ${ALIGN_VECT}

${CUT}: cut.c hash.c fasta.c
	mkdir -p ${BUILD}
	${CC} ${FLAGS} $^ -o $@

${ALIGN_STANDARD}: align_standard.c nw_diag.c ${COMMON}
	mkdir -p ${BUILD}
	${CC} ${FLAGS} $^ -o $@
	${CC} ${FLAGS} -DTRACEBACK -DSHOW_SEQ $^ -o $@_show_seq

${ALIGN_HASH}: align_hash.c hash.c nw_diag.c ${COMMON}
	mkdir -p ${BUILD}
	${CC} ${FLAGS} $^ -o $@
	${CC} ${FLAGS} -DTRACEBACK -DSHOW_SEQ $^ -o $@_show_seq

${ALIGN_VECT}: align_vect.c hash.c nw_diag.c nw_vect.c ${COMMON}
	mkdir -p ${BUILD}
	${CC} ${FLAGS} ${FLAGS_AVX} $^ -o $@
	${CC} ${FLAGS} ${FLAGS_AVX} -DREPEAT=${REPEAT} $^ -o $@_${REPEAT}
	${CC} ${FLAGS} ${FLAGS_AVX} -DTRACEBACK $^ -o $@_traceback
	${CC} ${FLAGS} ${FLAGS_AVX} -DTRACEBACK -DSHOW_SEQ $^ -o $@_show_seq
	${CC} ${FLAGS} ${FLAGS_AVX} -DKMER_SIZE=20 $^ -o $@_k20
	${CC} ${FLAGS} ${FLAGS_AVX} -DKMER_SIZE=20 -DREPEAT=${REPEAT} $^ -o $@_k20_${REPEAT}
	${CC} ${FLAGS} ${FLAGS_AVX} -DKMER_SIZE=20 -DTRACEBACK -DSHOW_SEQ $^ -o $@_k20_show_seq
	# ${CC} ${FLAGS} ${FLAGS_AVX} -DKMER_SIZE=26 $^ -o $@_k26
	# ${CC} ${FLAGS} ${FLAGS_AVX} -DKMER_SIZE=26 -DREPEAT=${REPEAT} $^ -o $@_k26_${REPEAT}
	# ${CC} ${FLAGS} ${FLAGS_AVX} -DBORDERLESS $^ -o $@_borderless
	# ${CC} ${FLAGS} ${FLAGS_AVX} -DBORDERLESS -DREPEAT=${REPEAT} $^ -o $@_borderless_${REPEAT}

clean:
	$(RM) -r ${BUILD}
