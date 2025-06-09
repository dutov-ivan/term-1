#include "validators.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils.h"

bool isGreater(double b, va_list args) {
  double a = va_arg(args, double);
  if (b <= a) {
    handleError("Верхня межа повинна бути більшою за нижню");
    return false;
  }
  return true;
}

bool isNotZero(double b, va_list args) {
  (void)args;
  if (b == 0) {
    handleError("Значення не може дорівнювати 0!");
    return false;
  }
  return true;
}

bool isNotContainZero(double b, va_list args) {
  double a = va_arg(args, double);
  if (a * b <= 0) {
    handleError("Проміжок не повинен містити 0!");
    return false;
  }
  return true;
}

bool isGreaterAndNotContainZero(double b, va_list args) {
  va_list argsCopy;
  va_copy(argsCopy, args);
  bool result = isNotContainZero(b, args) && isGreater(b, argsCopy);
  va_end(argsCopy);
  return result;
}
