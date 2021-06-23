#ifndef __HASH_H__
#define __HASH_H__

#include <stdbool.h>
#include <stdint.h>

#ifndef KMER_SIZE
#define KMER_SIZE (16)
#endif

#ifndef HASH_SIZE
#define HASH_SIZE (5)
#endif

#ifndef GAP_SIZE
#define GAP_SIZE (20)
#endif

#define MASK ((1 << ((KMER_SIZE - 1) << 1)) - 1)

typedef uint16_t hash_t;
typedef uint32_t kmer_t;
typedef uint16_t value_t;

typedef struct _list_t list_t;

struct _list_t {
  kmer_t kmer;
  value_t pos;
  list_t *next;
};

kmer_t code(char c);
kmer_t init_kmer(char *sequence);

hash_t hash(kmer_t k);

bool ht_contains(list_t **ht, kmer_t kmer);
void ht_add(list_t **ht, kmer_t kmer, value_t pos);
void ht_empty(list_t **ht);

#endif /* __HASH_H__ */
