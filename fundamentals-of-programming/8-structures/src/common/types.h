#ifndef TYPES_H
#define TYPES_H

#include <stdarg.h>
#include <stdbool.h>

typedef enum {
  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,
  WITHIN_RANGE
} RangeCheckResult;

typedef struct {
  double min, max;
  bool isMinIncluded, isMaxIncluded;
} DoubleRange;

typedef bool (*DoubleValidation)(double value, va_list args);

typedef struct {
  int min, max;
  bool isMinIncluded, isMaxIncluded;
} IntRange;

typedef bool (*IntValidation)(int value, va_list args);

#endif
