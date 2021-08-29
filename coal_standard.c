#include "fasta.h"
#include "nw.h"
#include "width.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  char *seq1;
  char *seq2;
  if (argc > 2) {
    load_one_seq(argv[1], &seq1);
    load_one_seq(argv[2], &seq2);
  } else if (argc > 1) {
    load_two_seq(argv[1], &seq1, &seq2);
  } else {
    fprintf(stderr, "Usage: %s <query.fasta> <target.fasta>\n", argv[0]);
    exit(1);
  }
  int l1 = strlen(seq1);
  int l2 = strlen(seq2);
  int w = width(l1, l2);
  int score = 0;
#ifdef REPEAT
  for (int r = 0; r < REPEAT; r++)
#endif
    score = nw_diag(seq1, l1, seq2, l2, w);
  printf("score: %d\n", score);
  free(seq1);
  free(seq2);

  return 0;
}
