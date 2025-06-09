#include "root.h"
#include "..//input/input.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>

long double approximate_kth_root(long double x, int k, long double e_target,
                                 int decimal_places, int is_quiet_mode,
                                 int is_return_delta, int iteration_limit) {

  if (k == 0) {
    if (!is_quiet_mode)
      display_error("Кореня нульового степеня не існує, спробуйте ще раз.");
    return INFINITY;
  }

  if (k % 2 == 0 && x < 0) {
    display_error(
        "Кореня парного степеня з від'ємного числа не існує, спробуйте ще "
        "раз");
    return INFINITY;
  }

  if (!is_quiet_mode) {
    printf("%10s %d\n", "", k);
    printf("Обчислюємо √%.*Lg ...\n", decimal_places, x);
  }

  int i = 0;

  long double y = (x >= 0) ? 1.0L : -1.0L;
  long double d = 0L;

  do {
    if (k > 0) {
      d = (x / powl(y, k - 1) - y) / k;
    } else {
      d = (1.0L / x / powl(y, -k - 1) - y) / -k;
    }
    y += d;

    if (!is_quiet_mode) {
      i++;
      printf("Ітерація %-*u   Δ = %-*.*Lf   y_%u = %-*.*Lf\n", MAX_CHARS_K, i,
             MAX_CHARS_X, decimal_places, d, i, MAX_CHARS_X, decimal_places, y);
    }
    if (i > iteration_limit) {
      return INFINITY;
    }
  } while (fabsl(d) >= e_target && !isinf(d) && !isnan(d));

  if (is_return_delta) {
    return d;
  }

  return y;
}
