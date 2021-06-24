#include "fasta.h"
#include "hash.h"
#include "nw.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int align_hash(char *s1, int n1, char *s2, int n2) {
  list_t *ht1[1 << HASH_SIZE];
  list_t *ht2[1 << HASH_SIZE];
  for (hash_t h = 0; h < (1 << HASH_SIZE); h++) {
    ht1[h] = NULL;
    ht2[h] = NULL;
  }
  int score = 0;
  int i0 = -1;
  int j0 = -1;
  kmer_t mask = MASK;
  int kmer1 = init_kmer(s1 + GAP_SIZE);
  int kmer2 = init_kmer(s2 + GAP_SIZE);
  int i = GAP_SIZE + KMER_SIZE - 1;
  int j = GAP_SIZE + KMER_SIZE - 1;
  while (i < n1 && j < n2) {
    kmer1 = ((kmer1 & mask) << 2) + code(s1[i]);
    kmer2 = ((kmer2 & mask) << 2) + code(s2[j]);
    ht_add(ht1, kmer1, i);
    ht_add(ht2, kmer2, j);

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
      int w = width(l1, l2);
#ifdef BORDERLESS
      if (i0 >= 0)
#endif
        score += nw_diag(s1 + i0 + 1, l1, s2 + j0 + 1, l2, w);
      score += KMER_SIZE * C_SAME;
#ifdef SHOW_SEQ
      for (int k = 1; k <= KMER_SIZE; k++) {
        printf("%c", s2[j - KMER_SIZE + k]);
      }
      printf("\n");
#endif
      ht_empty(ht2);
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
      int w = width(l1, l2);
#ifdef BORDERLESS
      if (i0 >= 0)
#endif
        score += nw_diag(s1 + i0 + 1, l1, s2 + j0 + 1, l2, w);
      score += KMER_SIZE * C_SAME;
#ifdef SHOW_SEQ
      for (int k = 1; k <= KMER_SIZE; k++) {
        printf("%c", s1[i - KMER_SIZE + k]);
      }
      printf("\n");
#endif
      ht_empty(ht1);
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
#ifndef BORDERLESS
  if (i0 < n1 && j0 < n2) {
    int l1 = n1 - 1 - i0;
    int l2 = n2 - 1 - j0;
    int w = width(l1, l2);
    score += nw_diag(s1 + i0 + 1, l1, s2 + j0 + 1, l2, w);
  }
#endif
  ht_empty(ht1);
  ht_empty(ht2);
  return score;
}

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
  int score = 0;
#ifdef REPEAT
  for (int r = 0; r < REPEAT; r++)
#endif
    score = align_hash(seq1, l1, seq2, l2);
  printf("score: %d\n", score);
  free(seq1);
  free(seq2);

  return 0;
}
