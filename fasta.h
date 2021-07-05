#ifndef __READ_SEQ_H__
#define __READ_SEQ_H__

#include <stdio.h>

char *read_sequence(FILE *fd, long max_len);

void load_one_seq(char *filename, char **seq);
void load_two_seq(char *filename, char **seq1, char **seq2);

#endif /* __READ_SEQ_H__ */
