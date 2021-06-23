#include "fasta.h"
#include <stdlib.h>
#include <string.h>

char *read_sequence(FILE *fd) {
  // read first sequence from file descriptor
  char *seq = calloc(MAX_SIZE, sizeof(char));
  if (seq == NULL) {
    fprintf(stderr, "Allocation failed\n");
    exit(1);
  }
  int seq_len = 0;
  while (fgets(seq + seq_len, MAX_SIZE - seq_len, fd)) {
    int line_len = strlen(seq + seq_len);
    if (line_len == 0) { // empty line
      continue;
    }
    if (seq[seq_len] == '>') { // ignore comment
      if (seq_len > 0) {
        seq[seq_len] = '\0';
        break; // stop after first sequence
      }
      continue;
    }
    if (seq[seq_len + line_len - 1] == '\n') { // ignore newline
      seq[seq_len + line_len - 1] = '\0';
      line_len--;
    }
    seq_len += line_len;
  }
  return seq;
}

void load_one_seq(char *filename, char **seq) {
  FILE *fd = fopen(filename, "r");
  if (fd == NULL) {
    fprintf(stderr, "Invalid filename: %s\n", filename);
    exit(1);
  }
  *seq = read_sequence(fd);
  fclose(fd);
}

void load_two_seq(char *filename, char **seq1, char **seq2) {
  FILE *fd = fopen(filename, "r");
  if (fd == NULL) {
    fprintf(stderr, "Invalid filename: %s\n", filename);
    exit(1);
  }
  *seq1 = read_sequence(fd);
  *seq2 = read_sequence(fd);
  fclose(fd);
}
