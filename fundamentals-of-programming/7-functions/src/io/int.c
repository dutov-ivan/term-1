#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/constants.h"
#include "../common/types.h"
#include "int.h"
#include "utils.h"

const IntRange INT_CHAR_COUNT_RANGE = {1, MAX_CHAR_COUNT_INT, true, true};

void showPromptWithinRange(const char *prompt, const IntRange *range) {
  printf("%s (від %d до %d): ", prompt, range->min, range->max);
}

void showRangeErrorInt(RangeCheckResult result, const IntRange *range) {
  switch (result) {
  case LESS:
    handleError("Значення має бути ≥ %d.", range->min);
    break;
  case LESS_EQUAL:
    handleError("Значення має бути > %d.", range->min);
    break;
  case GREATER:
    handleError("Значення має бути ≤ %d.", range->max);
    break;
  case GREATER_EQUAL:
    handleError("Значення має бути < %d.", range->max);
    break;
  default:
    break;
  }
}

// Range Checking Functions
RangeCheckResult validateRangeInt(int value, const IntRange *range) {
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

void printIntRange(const char *name, const IntRange *range) {
  printf("%d", range->min);
  if (range->isMinIncluded) {
    printf(" ≤ ");
  } else {
    printf(" < ");
  }

  printf("%s", name);

  if (range->isMaxIncluded) {
    printf(" ≤ ");
  } else {
    printf(" < ");
  }

  printf("%d\n", range->max);
}

void printIntCharacterCountRange() {
  printIntRange("кількість символів у цілих числах", &INT_CHAR_COUNT_RANGE);
}

RangeCheckResult validateOverflowInt(long int value) {
  if (value > MAX_VALUE_INT)
    return GREATER;
  if (value < MIN_VALUE_INT)
    return LESS;
  return WITHIN_RANGE;
}

int readIntQuiet(int *value) {
  char *input = readInput(MAX_CHAR_COUNT_INT, MAX_CHAR_COUNT_INT);

  if (input == NULL) {
    return FAILURE;
  }

  if (!isInputWithinLength(input)) {
    handleErrorOverlength(MAX_CHAR_COUNT_INT);
    clearStdin();
    free(input);
    return FAILURE;
  }

  char *endptr;
  long int tempValue = strtol(input, &endptr, 10);

  if (!isInputNumberAfterConversion(endptr, input)) {
    handleErrorNotNumber();
    free(input);
    return FAILURE;
  }

  RangeCheckResult globalCheck = validateOverflowInt(tempValue);
  if (globalCheck != WITHIN_RANGE) {
    handleErrorOverflow();
    free(input);
    return FAILURE;
  }

  *value = (int)tempValue;

  free(input);
  return SUCCESS;
}

int readInt(int *value, const char *prompt) {
  showPrompt(prompt);
  return readIntQuiet(value);
}

int readIntWithinRange(int *value, const char *prompt, const IntRange *range) {
  showPromptWithinRange(prompt, range);
  if (readIntQuiet(value) == FAILURE) {
    return FAILURE;
  };

  RangeCheckResult rangeCheck = validateRangeInt(*value, range);
  if (rangeCheck != WITHIN_RANGE) {
    showRangeErrorInt(rangeCheck, range);
    return FAILURE;
  }
  return SUCCESS;
}

int readIntWithinRangeWithValidation(const char *prompt, const IntRange *range,
                                     IntValidation additionalCheck, ...) {
  int value;
  bool isValid = false;

  va_list args;

  while (!isValid) {
    if (readIntWithinRange(&value, prompt, range) == SUCCESS) {
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

int readIntWithValidation(const char *prompt, IntValidation additionalCheck,
                          ...) {
  int value;
  bool isValid = false;

  va_list args;

  while (!isValid) {
    if (readInt(&value, prompt) == SUCCESS) {
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
