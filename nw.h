#ifndef __NW_H__
#define __NW_H__

#include <stdint.h>

#ifndef C_SAME
#define C_SAME (2)
#endif

#ifndef C_SUB
#define C_SUB (-3)
#endif

#ifndef C_GAP
#define C_GAP (-2)
#endif

#ifndef T_DIAG
#define T_DIAG (1)
#endif

#ifndef T_LEFT
#define T_LEFT (2)
#endif

#ifndef T_UP
#define T_UP (3)
#endif

typedef int16_t score_t;

#ifndef N_INF
#define N_INF (-(INT16_MAX >> 1))
#endif

#ifndef W_SIZE
// 2 * w + 2 = 16
#define W_SIZE (7)
#endif

score_t nw_diag(char *s1, int l1, char *s2, int l2, int w);
score_t nw_vect(char *s1, int l1, char *s2, int l2);

#endif /* __NW_H__ */
