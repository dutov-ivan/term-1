#include "io.h"
#include "utils.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

// INFO: Only for size_t for now
bool isNotZero(void *value, va_list args) {
  size_t val = *(size_t *)value;
  if (val == 0) {
    handleError("Value can't be 0!");
    return false;
  }
  return true;
}
