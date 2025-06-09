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
  bool is_min_included, is_max_included;
} DoubleRange;

typedef struct {
  int min, max;
  bool is_min_included, is_max_included;
} IntRange;

typedef bool (*DoubleValidation)(double value, va_list args);
typedef bool (*IntValidation)(int value, va_list args);

#endif
