#include "utils.h"
#include <stdlib.h>

int width(int l1, int l2) { return (abs(l1 - l2) >> 1) + 3; }
int width_large(int l1, int l2) { return abs(l1 - l2) + 5; }
