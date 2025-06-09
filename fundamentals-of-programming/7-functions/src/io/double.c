#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/constants.h"
#include "../common/types.h"
#include "double.h"
#include "utils.h"

// OUTPUT
void showPromptDoubleWithinRange(const char *prompt, const DoubleRange *range) {
  printf("%s (від %lg до %lg): ", prompt, range->min, range->max);
}

void showRangeErrorDouble(RangeCheckResult result, const DoubleRange *range) {
  switch (result) {
  case LESS:
    handleError("Значення має бути ≥ %lg.", range->min);
    break;
  case LESS_EQUAL:
    handleError("Значення має бути > %lg.", range->min);
    break;
  case GREATER:
    handleError("Значення має бути ≤ %lg.", range->max);
    break;
  case GREATER_EQUAL:
    handleError("Значення має бути < %lg.", range->max);
    break;
  default:
    printf("Значення знаходиться в заданому проміжку.");
    break;
  }
}

// RANGES
RangeCheckResult validateRangeDouble(double value, const DoubleRange *range) {
  if (range->isMinIncluded && value < range->min)
    return LESS;
  if (!range->isMinIncluded && value <= range->min)
    return LESS_EQUAL;
  if (range->isMaxIncluded && value > range->max)
    return GREATER;
  if (!range->isMaxIncluded && value >= range->max)
    return GREATER_EQUAL;
  return WITHIN_RANGE;
}

void printDoubleRange(const char *name, const DoubleRange *range) {
  printf("%g %s %s %s %g\n", range->min, (range->isMinIncluded) ? "≤" : "<",
         name, (range->isMaxIncluded) ? "≤" : "<", range->max);
}

void printDoubleCharacterCountRange() {
  printf("0 < кількість символів у дійсних числах ≤ %d", MAX_CHAR_COUNT_DOUBLE);
}

void printDoublePrecisionLimit() {
  printf("Будь-які дійсні числа приймаються з точністю, не меншою за %g\n",
         TOLERANCE_DOUBLE);
}

RangeCheckResult validateOverflowDouble(double value) {
  if (errno == ERANGE) {
    if (fabs(value) == OVERFLOW_ABS_VALUE_DOUBLE)
      return GREATER_EQUAL;
    if (fabs(value) < MIN_ABS_VALUE_DOUBLE)
      return LESS_EQUAL;
  }
  return WITHIN_RANGE;
}

// UTILS
double truncateToPrecisionDouble(double num, int decimalPlaces) {
  double factor = pow(10.0, (double)decimalPlaces);
  return trunc(num * factor) / factor;
}

void printTruncatedDouble(double num, int decimalPlaces) {
  printf("%.*lf", decimalPlaces, truncateToPrecisionDouble(num, decimalPlaces));
}

bool isDoublePrecise(double value) {
  if (isinf(value) || isnan(value)) {
    return false;
  }

  if (value == 0.0) {
    return true;
  }

  double scale = pow(10.0, MAX_DIGITS_DOUBLE);
  double scaledValue = round(value * scale);
  double roundedValue = scaledValue / scale;

  double tolerance = fabs(value) * TOLERANCE_DOUBLE;

  return fabs(value - roundedValue) < tolerance;
}

// INPUT

int readDoubleQuiet(double *value) {
  errno = 0;

  char *input = readInput(INITIAL_CHAR_COUNT_DOUBLE, MAX_CHAR_COUNT_DOUBLE);

  if (input == NULL) {
    return FAILURE;
  }

  replaceCommasWithDots(input);

  char *endptr;
  *value = strtod(input, &endptr);

  if (!isInputNumberAfterConversion(endptr, input)) {
    handleErrorNotNumber();
    free(input);
    return FAILURE;
  }

  RangeCheckResult globalCheck = validateOverflowDouble(*value);
  if (globalCheck != WITHIN_RANGE) {
    handleErrorOverflow();
    free(input);
    return FAILURE;
  }

  if (!isDoublePrecise(*value)) {
    warnNotPrecise(MAX_DIGITS_DOUBLE);
  }

  free(input);

  return SUCCESS;
}

int readDouble(double *value, const char *prompt) {
  showPrompt(prompt);
  return readDoubleQuiet(value);
}

int readDoubleWithinRange(double *value, const char *prompt,
                          const DoubleRange *range) {
  showPromptDoubleWithinRange(prompt, range);

  if (readDoubleQuiet(value) == FAILURE) {
    return FAILURE;
  }

  RangeCheckResult rangeCheck = validateRangeDouble(*value, range);
  if (rangeCheck != WITHIN_RANGE) {
    showRangeErrorDouble(rangeCheck, range);
    return FAILURE;
  }
  return SUCCESS;
}

double readDoubleWithinRangeWithValidation(const char *prompt,
                                           const DoubleRange *range,
                                           DoubleValidation additionalCheck,
                                           ...) {
  double value;
  bool isValid = false;

  va_list args;

  while (!isValid) {
    if (readDoubleWithinRange(&value, prompt, range) == SUCCESS) {
      if (additionalCheck != NULL) {
        va_start(args, additionalCheck);
        isValid = additionalCheck(value, args);
        va_end(args);

      } else {
        isValid = true;
      }
    }
  }

  return value;
}
