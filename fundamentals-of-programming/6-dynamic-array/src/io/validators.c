#include "validators.h"

#include <stdarg.h>
#include <stdbool.h>

#include "utils.h"

bool is_greater(double b, va_list args) {
  double a = va_arg(args, double);
  bool is_greater = b > a;
  if (!is_greater) {
    handle_error("Верхня межа повинна бути більшою за нижню");
  }
  return is_greater;
}
