#ifndef __READ_SEQ_H__
#define __READ_SEQ_H__

#include <stdio.h>

#ifndef MAX_SIZE
#define MAX_SIZE (25000)
#endif

char *read_sequence(FILE *fd);

void load_one_seq(char *filename, char **seq);
void load_two_seq(char *filename, char **seq1, char **seq2);

#endif /* __READ_SEQ_H__ */
