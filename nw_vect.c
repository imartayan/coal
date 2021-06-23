#include "nw.h"
#include <immintrin.h>
#include <mmintrin.h>
#include <nmmintrin.h>
#include <smmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__m256i shiftL_epi16(__m256i v) {
  // shift by 16 bits to the left
  int16_t tmp = (int16_t)_mm256_extract_epi16(v, 8);
  __m256i res = _mm256_bsrli_epi128(v, 2); // tricky
  res = _mm256_insert_epi16(res, tmp, 7);
  return res;
}

__m256i shiftR_epi16(__m256i v) {
  // shift by 16 bits to the right
  int16_t tmp = (int16_t)_mm256_extract_epi16(v, 7);
  __m256i res = _mm256_bslli_epi128(v, 2); // tricky
  res = _mm256_insert_epi16(res, tmp, 8);
  return res;
}

score_t nw_vect(char *s1, int l1, char *s2, int l2) {
  const int w = W_SIZE;

  __m256i v_diag_even = _mm256_set1_epi16(N_INF);
  __m256i v_diag_odd = _mm256_set1_epi16(N_INF);
  __m256i c_same = _mm256_set1_epi16(C_SAME);
  __m256i c_sub = _mm256_set1_epi16(C_SUB);
  __m256i c_gap = _mm256_set1_epi16(C_GAP);
#ifdef TRACEBACK
  int16_t traceback[l1 + l2 + 1][16]; // size must be the same
  __m256i t_diag = _mm256_set1_epi16(T_DIAG);
  __m256i t_left = _mm256_set1_epi16(T_LEFT);
  __m256i t_up = _mm256_set1_epi16(T_UP);
#endif

  v_diag_even = _mm256_insert_epi16(v_diag_even, 0, 8); // w + 1

  __m256i v_s1 = _mm256_setzero_si256();
  __m256i v_s2 = _mm256_setzero_si256();
  int i = -w - 3;
  int j = -2 - i;
#define INIT_VECT(k)                                                           \
  {                                                                            \
    i++;                                                                       \
    j--;                                                                       \
    if (0 <= i && i < l1) {                                                    \
      v_s1 = _mm256_insert_epi16(v_s1, s1[i], k);                              \
    } else {                                                                   \
      v_s1 = _mm256_insert_epi16(v_s1, '1', k);                                \
    }                                                                          \
    if (0 <= j && j < l2) {                                                    \
      v_s2 = _mm256_insert_epi16(v_s2, s2[j], k);                              \
    } else {                                                                   \
      v_s2 = _mm256_insert_epi16(v_s2, '2', k);                                \
    }                                                                          \
  }
  INIT_VECT(0)
  INIT_VECT(1)
  INIT_VECT(2)
  INIT_VECT(3)
  INIT_VECT(4)
  INIT_VECT(5)
  INIT_VECT(6)
  INIT_VECT(7)
  INIT_VECT(8)
  INIT_VECT(9)
  INIT_VECT(10)
  INIT_VECT(11)
  INIT_VECT(12)
  INIT_VECT(13)
  INIT_VECT(14)
  INIT_VECT(15)
  j += 15;

  // Compute score diag with dynamic programming
  for (int d = 1; d <= l1 + l2; d++) {
    if (d & 1) { // odd
      i++;
      v_s1 = shiftL_epi16(v_s1);
      if (0 <= i && i < l1) {
        v_s1 = _mm256_insert_epi16(v_s1, s1[i], 15);
      } else {
        v_s1 = _mm256_insert_epi16(v_s1, '1', 15);
      }

      __m256i v_diag = _mm256_set1_epi16(N_INF);
#ifdef TRACEBACK
      __m256i v_traceback = _mm256_setzero_si256();
#endif
      __m256i v_cmp = _mm256_cmpeq_epi16(v_s1, v_s2);
      __m256i v_match = _mm256_blendv_epi8(c_sub, c_same, v_cmp);
      v_match = _mm256_add_epi16(v_diag_odd, v_match);
      __m256i v_ins = _mm256_add_epi16(v_diag_even, c_gap);
      __m256i v_rem = shiftL_epi16(v_ins);
      v_rem = _mm256_insert_epi16(v_rem, N_INF, 15);

      v_cmp = _mm256_cmpgt_epi16(v_match, v_diag);
      v_diag = _mm256_blendv_epi8(v_diag, v_match, v_cmp);
#ifdef TRACEBACK
      v_traceback = _mm256_blendv_epi8(v_traceback, t_diag, v_cmp);
#endif

      v_cmp = _mm256_cmpgt_epi16(v_rem, v_diag);
      v_diag = _mm256_blendv_epi8(v_diag, v_rem, v_cmp);
#ifdef TRACEBACK
      v_traceback = _mm256_blendv_epi8(v_traceback, t_left, v_cmp);
#endif

      v_cmp = _mm256_cmpgt_epi16(v_ins, v_diag);
      v_diag = _mm256_blendv_epi8(v_diag, v_ins, v_cmp);
#ifdef TRACEBACK
      v_traceback = _mm256_blendv_epi8(v_traceback, t_up, v_cmp);
#endif

      v_diag = _mm256_insert_epi16(v_diag, N_INF, 0);
      v_diag = _mm256_insert_epi16(v_diag, N_INF, 15);

      v_diag_odd = v_diag;
#ifdef TRACEBACK
      memcpy(traceback[d], &v_traceback, 256 >> 3);
#endif
    } else { // even
      j++;
      v_s2 = shiftR_epi16(v_s2);
      if (0 <= j && j < l2) {
        v_s2 = _mm256_insert_epi16(v_s2, s2[j], 0);
      } else {
        v_s2 = _mm256_insert_epi16(v_s2, '2', 0);
      }

      __m256i v_diag = _mm256_set1_epi16(N_INF);
#ifdef TRACEBACK
      __m256i v_traceback = _mm256_setzero_si256();
#endif
      __m256i v_cmp = _mm256_cmpeq_epi16(v_s1, v_s2);
      __m256i v_match = _mm256_blendv_epi8(c_sub, c_same, v_cmp);
      v_match = _mm256_add_epi16(v_diag_even, v_match);
      __m256i v_rem = _mm256_add_epi16(v_diag_odd, c_gap);
      __m256i v_ins = shiftR_epi16(v_rem);
      v_ins = _mm256_insert_epi16(v_ins, N_INF, 0);

      v_cmp = _mm256_cmpgt_epi16(v_match, v_diag);
      v_diag = _mm256_blendv_epi8(v_diag, v_match, v_cmp);
#ifdef TRACEBACK
      v_traceback = _mm256_blendv_epi8(v_traceback, t_diag, v_cmp);
#endif

      v_cmp = _mm256_cmpgt_epi16(v_rem, v_diag);
      v_diag = _mm256_blendv_epi8(v_diag, v_rem, v_cmp);
#ifdef TRACEBACK
      v_traceback = _mm256_blendv_epi8(v_traceback, t_left, v_cmp);
#endif

      v_cmp = _mm256_cmpgt_epi16(v_ins, v_diag);
      v_diag = _mm256_blendv_epi8(v_diag, v_ins, v_cmp);
#ifdef TRACEBACK
      v_traceback = _mm256_blendv_epi8(v_traceback, t_up, v_cmp);
#endif

      v_diag = _mm256_insert_epi16(v_diag, N_INF, 0);

      v_diag_even = v_diag;
#ifdef TRACEBACK
      memcpy(traceback[d], &v_traceback, 256 >> 3);
#endif
    }
  }

  int d = l1 + l2;
  int delta = l1 >= l2 ? (l1 - l2) >> 1 : -((l2 - l1 + 1) >> 1); // tricky
  int k = w + 1 + delta;
  score_t *diag = (d & 1) ? (score_t *)&v_diag_odd : (score_t *)&v_diag_even;
  score_t score = diag[k];

#ifdef TRACEBACK
  // Build sequence from traceback
  char *r1 = malloc(l1 + l2);
  char *r2 = malloc(l1 + l2);
  char *r3 = malloc(l1 + l2);
  int start = l1 + l2 - 1;
  r1[start] = '\0';
  r2[start] = '\0';
  r3[start] = '\0';
  i = l1 - 1;
  j = l2 - 1;
  while (d > 0) {
    if (traceback[d][k] == T_DIAG) {
      start--;
      r1[start] = s1[i];
      r2[start] = s2[j];
      if (s1[i] == s2[j]) {
        r3[start] = '|';
      } else {
        r3[start] = ' ';
      }
      d -= 2;
      i -= 1;
      j -= 1;
    } else if (traceback[d][k] == T_UP) {
      start--;
      r1[start] = s1[i];
      r2[start] = '_';
      r3[start] = ' ';
      if (!(d & 1)) {
        k -= 1;
      }
      d -= 1;
      i -= 1;
    } else if (traceback[d][k] == T_LEFT) {
      start--;
      r1[start] = '_';
      r2[start] = s2[j];
      r3[start] = ' ';
      if (d & 1) {
        k += 1;
      }
      d -= 1;
      j -= 1;
    } else {
      break;
    }
  }
#ifdef SHOW_SEQ
  printf("%s\n%s\n%s\n", r1 + start, r3 + start, r2 + start);
#endif
  free(r1);
  free(r2);
  free(r3);
#endif

  return score;
}
