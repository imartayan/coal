#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char base[4] = {'A', 'T', 'C', 'G'};

kmer_t code(char c) {
  // Code corresponding to a given base (requires only 2 bits)
  switch (c) {
  case 'A':
    return 0;
  case 'T':
    return 1;
  case 'C':
    return 2;
  case 'G':
    return 3;
  default:
    return 0;
  }
}

kmer_t init_kmer(char *sequence) {
  // Initialize kmer from sequence
  kmer_t kmer = 0;
  for (uint8_t i = 0; i < KMER_SIZE - 1; i++) {
    kmer = (kmer << 2) + code(sequence[i]);
  }
  return kmer;
}

hash_t hash(kmer_t k) {
  // Hash function : XOR between each block of the kmer
  hash_t h = 0;
  for (uint8_t i = 0; i < (KMER_SIZE << 1); i += HASH_SIZE) {
    h ^= k & ((1 << HASH_SIZE) - 1);
    k >>= HASH_SIZE;
  }
  return h;
}

bool ht_contains(list_t **ht, kmer_t kmer) {
  hash_t h = hash(kmer);
  list_t *cell = ht[h];
  list_t *prev = cell;
  while (cell != NULL && cell->kmer != kmer) {
    prev = cell;
    cell = cell->next;
  }
  if (cell == NULL) {
    return false;
  } else if (cell != prev) {
    // Move cell to the head of the list
    prev->next = cell->next;
    cell->next = ht[h];
    ht[h] = cell;
  }
  return true;
}

void ht_add(list_t **ht, kmer_t kmer, value_t pos) {
  if (!ht_contains(ht, kmer)) {
    hash_t h = hash(kmer);
    list_t *cell = malloc(sizeof(list_t));
    if (cell == NULL) {
      fprintf(stderr, "Cell allocation failed\n");
      exit(1);
    }
    cell->kmer = kmer;
    cell->pos = pos;
    cell->next = ht[h];
    ht[h] = cell;
  }
}

void ht_empty(list_t **ht) {
  for (hash_t h = 0; h < (1 << HASH_SIZE); h++) {
    list_t *cell = ht[h];
    while (cell != NULL) {
      list_t *next = cell->next;
      free(cell);
      cell = next;
    }
    ht[h] = NULL;
  }
}
