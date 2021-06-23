#include "nw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

score_t nw_diag(char *s1, int l1, char *s2, int l2, int w) {
  score_t diag_init[(w << 1) + 3];
  for (int k = 0; k < (w << 1) + 3; k++) {
    diag_init[k] = N_INF;
  }
  score_t diag_even[(w << 1) + 3];
  memcpy(diag_even, diag_init, ((w << 1) + 3) * sizeof(score_t));
  score_t diag_odd[(w << 1) + 2];
  memcpy(diag_even, diag_init, ((w << 1) + 2) * sizeof(score_t));
#ifdef TRACEBACK
  int8_t traceback[l1 + l2 + 1][(w << 1) + 3];
#endif

  diag_even[w + 1] = 0;

  // Compute score diag with dynamic programming
  for (int d = 1; d <= l1 + l2; d++) {
    if (d & 1) { // odd
      score_t curr_diag[(w << 1) + 2];
      memcpy(curr_diag, diag_init, ((w << 1) + 2) * sizeof(score_t));
      for (int k = 1; k < (w << 1) + 1; k++) {
        int i = k - w + (d >> 1);
        int j = d - i;
        score_t ins = diag_even[k] + C_GAP;
        score_t rem = diag_even[k + 1] + C_GAP;
        if (ins >= curr_diag[k]) {
          curr_diag[k] = ins;
#ifdef TRACEBACK
          traceback[d][k] = T_UP;
#endif
        }
        if (rem >= curr_diag[k]) {
          curr_diag[k] = rem;
#ifdef TRACEBACK
          traceback[d][k] = T_LEFT;
#endif
        }
        if (1 <= i && i <= l1 && 1 <= j && j <= l2) {
          score_t match =
              diag_odd[k] + (s1[i - 1] == s2[j - 1] ? C_SAME : C_SUB);
          if (match >= curr_diag[k]) {
            curr_diag[k] = match;
#ifdef TRACEBACK
            traceback[d][k] = T_DIAG;
#endif
          }
        }
      }
      memcpy(diag_odd, curr_diag, ((w << 1) + 2) * sizeof(score_t));
    } else { // even
      score_t curr_diag[(w << 1) + 3];
      memcpy(curr_diag, diag_init, ((w << 1) + 3) * sizeof(score_t));
      for (int k = 1; k < (w << 1) + 2; k++) {
        int i = k - (w + 1) + (d >> 1);
        int j = d - i;
        score_t ins = diag_odd[k - 1] + C_GAP;
        score_t rem = diag_odd[k] + C_GAP;
        if (ins >= curr_diag[k]) {
          curr_diag[k] = ins;
#ifdef TRACEBACK
          traceback[d][k] = T_UP;
#endif
        }
        if (rem >= curr_diag[k]) {
          curr_diag[k] = rem;
#ifdef TRACEBACK
          traceback[d][k] = T_LEFT;
#endif
        }
        if (1 <= i && i <= l1 && 1 <= j && j <= l2) {
          score_t match =
              diag_even[k] + (s1[i - 1] == s2[j - 1] ? C_SAME : C_SUB);
          if (match >= curr_diag[k]) {
            curr_diag[k] = match;
#ifdef TRACEBACK
            traceback[d][k] = T_DIAG;
#endif
          }
        }
      }
      memcpy(diag_even, curr_diag, ((w << 1) + 3) * sizeof(score_t));
    }
  }

  int d = l1 + l2;
  int delta = l1 >= l2 ? (l1 - l2) >> 1 : -((l2 - l1 + 1) >> 1); // tricky
  int k = w + 1 + delta;
  score_t score = (d & 1) ? diag_odd[k] : diag_even[k];

#ifdef TRACEBACK
  // Build sequence from traceback
  char *r1 = malloc(l1 + l2);
  char *r2 = malloc(l1 + l2);
  char *r3 = malloc(l1 + l2);
  int start = l1 + l2 - 1;
  r1[start] = '\0';
  r2[start] = '\0';
  r3[start] = '\0';
  int i = l1 - 1;
  int j = l2 - 1;
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
