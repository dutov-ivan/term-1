#ifndef DOUBLE_H
#define DOUBLE_H

#include "../common/types.h"
#include <float.h>

#define MIN_ABS_VALUE_DOUBLE DBL_MIN
#define MAX_ABS_VALUE_DOUBLE DBL_MAX
#define MAX_CHAR_COUNT_DOUBLE DBL_MAX_10_EXP
#define INITIAL_CHAR_COUNT_DOUBLE 16
#define MAX_DIGITS_DOUBLE DBL_DIG
#define TOLERANCE_DOUBLE DBL_EPSILON
#define OVERFLOW_ABS_VALUE_DOUBLE HUGE_VAL

void printDoubleRange(const char *name, const DoubleRange *range);
void printDoubleCharacterCountRange();
void printDoublePrecisionLimit();
void printTruncatedDouble(double num, int decimalPlaces);
int readDoubleWithinRange(double *value, const char *prompt,
                          const DoubleRange *range);
double readDoubleWithinRangeWithValidation(const char *prompt,
                                           const DoubleRange *range,
                                           DoubleValidation additionalCheck,
                                           ...);

#endif
