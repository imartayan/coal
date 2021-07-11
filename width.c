#include "width.h"
#include <stdlib.h>

int width(int l1, int l2) {
  int lmax = l1 > l2 ? l1 : l2;
  int lmin = l1 < l2 ? l1 : l2;
  int diff = (lmax - lmin + 1) >> 1;
  int margin = 3 + (lmax >> 7);
  return diff + margin;
}
