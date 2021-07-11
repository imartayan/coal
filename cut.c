#include "fasta.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cut(char *s1, int n1, char *s2, int n2, char *out1, char *out2) {
  list_t *ht1[1 << HASH_SIZE];
  list_t *ht2[1 << HASH_SIZE];
  for (hash_t h = 0; h < (1 << HASH_SIZE); h++) {
    ht1[h] = NULL;
    ht2[h] = NULL;
  }
  int i0 = -1;
  int j0 = -1;
  kmer_t mask = MASK;
  kmer_t kmer1 = init_kmer(s1);
  kmer_t kmer2 = init_kmer(s2);
  int i = KMER_SIZE - 1;
  int j = KMER_SIZE - 1;
  int start1 = 0;
  int start2 = 0;

  while (i < n1 && j < n2) {
    kmer1 = ((kmer1 & mask) << 2) + code(s1[i]);
    kmer2 = ((kmer2 & mask) << 2) + code(s2[j]);
    if (!ht_contains(ht1, kmer1)) {
      ht_add(ht1, kmer1, i);
    } else {
      ht_remove(ht1, kmer1);
    }
    if (!ht_contains(ht2, kmer2)) {
      ht_add(ht2, kmer2, j);
    } else {
      ht_remove(ht2, kmer2);
    }

    if (ht_contains(ht1, kmer2)) {
      hash_t h = hash(kmer2);
      int i1 = ht1[h]->pos;
      int l1 = i1 - KMER_SIZE - i0;
      int l2 = j - KMER_SIZE - j0;
      if (l1 <= 0 || l2 <= 0) {
        i += 1;
        j += 1;
        continue;
      }
      ht_empty(ht2);
      if (i0 < 0) {
        start1 = i1 - KMER_SIZE + 1;
        start2 = j - KMER_SIZE + 1;
      }
      i0 = i1;
      j0 = j;
      i += 1;
      j += GAP_SIZE + 1;
      kmer2 = init_kmer(s2 + j);
      j += KMER_SIZE - 1;
    } else if (ht_contains(ht2, kmer1)) {
      hash_t h = hash(kmer1);
      int j1 = ht2[h]->pos;
      int l1 = i - KMER_SIZE - i0;
      int l2 = j1 - KMER_SIZE - j0;
      if (l1 <= 0 || l2 <= 0) {
        i += 1;
        j += 1;
        continue;
      }
      ht_empty(ht1);
      if (i0 < 0) {
        start1 = i - KMER_SIZE + 1;
        start2 = j1 - KMER_SIZE + 1;
      }
      i0 = i;
      j0 = j1;
      i += GAP_SIZE + 1;
      kmer1 = init_kmer(s1 + i);
      i += KMER_SIZE - 1;
      j += 1;
    } else {
      i += 1;
      j += 1;
    }
  }
  ht_empty(ht1);
  ht_empty(ht2);
  int stop1 = i0;
  int stop2 = j0;

  if (start1 + KMER_SIZE <= stop1 && start2 + KMER_SIZE <= stop2) {
    FILE *fd = fopen(out1, "w");
    if (fd == NULL) {
      fprintf(stderr, "Invalid filename: %s\n", out1);
      exit(1);
    }
    fprintf(fd, ">cut 1\n");
    for (int k = start1; k <= stop1; k++) {
      fprintf(fd, "%c", s1[k]);
    }
    fclose(fd);

    fd = fopen(out2, "w");
    if (fd == NULL) {
      fprintf(stderr, "Invalid filename: %s\n", out2);
      exit(1);
    }
    fprintf(fd, ">cut 2\n");
    for (int k = start2; k <= stop2; k++) {
      fprintf(fd, "%c", s2[k]);
    }
    fclose(fd);
  } else {
    fprintf(stderr, "Failed to find two solid kmers");
    exit(1);
  }
}

int main(int argc, char **argv) {
  char *seq1;
  char *seq2;
  char *out1;
  char *out2;
  if (argc > 4) {
    load_one_seq(argv[1], &seq1);
    load_one_seq(argv[2], &seq2);
    out1 = argv[3];
    out2 = argv[4];
  } else if (argc > 3) {
    load_two_seq(argv[1], &seq1, &seq2);
    out1 = argv[2];
    out2 = argv[3];
  } else {
    fprintf(stderr, "Usage: %s <seq1> <seq2> <output1> <output2>\n", argv[0]);
    exit(1);
  }
  int l1 = strlen(seq1);
  int l2 = strlen(seq2);
  cut(seq1, l1, seq2, l2, out1, out2);
  free(seq1);
  free(seq2);

  return 0;
}
