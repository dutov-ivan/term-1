#include "validators.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils.h"

// Validation functions

bool isGreaterOrEqual(double b, va_list args) {
  double a = va_arg(args, double);
  if (b < a) {
    handleError("Верхня межа повинна бути більшою за нижню");
    return false;
  }
  return true;
}

bool isLess(double b, va_list args) {
  double a = va_arg(args, double);
  if (b >= a) {
    handleError("Значення має бути < %g", a);
    return false;
  }
  return true;
}
