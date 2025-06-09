#include "square.h"

int is_perfect_square(int num) {
  int l = 1, r = num;
  int m;
  while (l <= r) {
    m = l + (r - l) / 2;
    if (m * m > num) {
      r = m - 1;
    } else if (m * m < num) {
      l = m + 1;
    } else {
      return 1;
    }
  }
  return 0;
}
