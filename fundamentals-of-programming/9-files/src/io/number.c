#include "io.h"
#include "utils.h"
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

const char *typeDescriptions[TYPE_COUNT] = {"floating-point decimal",
                                            "length of arrays"};

size_t getTypeSize(NumberType type) {
  switch (type) {
  case TYPE_SIZE_T:
    return sizeof(size_t);
  case TYPE_DOUBLE:
    return sizeof(double);
  default:
    handleError("Number type not implemented.");
    return 0;
  }
}

int compareDouble(const void *a, const void *b) {
  if (*(double *)a < *(double *)b)
    return -1;
  if (*(double *)a > *(double *)b)
    return 1;
  return 0;
}

int compareSizeT(const void *a, const void *b) {
  if (*(size_t *)a < *(size_t *)b)
    return -1;
  if (*(size_t *)a > *(size_t *)b)
    return 1;
  return 0;
}

NumberRange *initializeNumberRange(NumberType type, void *min, void *max,
                                   bool isMinIncluded, bool isMaxIncluded,
                                   const char *valueName) {
  NumberRange *range = malloc(sizeof(NumberRange));
  if (!range) {
    handleErrorMemoryAllocation("range");
    return NULL;
  }

  range->type = type;
  size_t typeSize = getTypeSize(type);

  range->min = malloc(typeSize);
  if (!range->min) {
    handleErrorMemoryAllocation("number range min");
    freeNumberRange(range);
    return NULL;
  }

  range->max = malloc(typeSize);
  if (!range->max) {
    handleError("Memory allocation failed for range min/max values.");
    freeNumberRange(range);
    return NULL;
  }

  switch (type) {
  case TYPE_SIZE_T:
    *(size_t *)range->min = *(size_t *)min;
    *(size_t *)range->max = *(size_t *)max;
    range->compare = compareSizeT;
    break;

  case TYPE_DOUBLE:
    *(double *)range->min = *(double *)min;
    *(double *)range->max = *(double *)max;
    range->compare = compareDouble;
    break;
  default:
    handleError("Unsupported type in range initialization.");
    freeNumberRange(range);
    return NULL;
  }

  range->isMinIncluded = isMinIncluded;
  range->isMaxIncluded = isMaxIncluded;
  range->valueName = strdup(valueName);
  if (!range->valueName) {
    handleErrorMemoryAllocation("number range value name");
    freeNumberRange(range);
    return NULL;
  }

  return range;
}

void freeNumberRange(NumberRange *range) {
  if (!range)
    return;
  if (range->min)
    free(range->min);
  if (range->max)
    free(range->max);
  if (range->valueName)
    free(range->valueName);
  free(range);
}

RangeCheckResult validateNumberRange(const void *value,
                                     const NumberRange *range) {
  const char *modal = "should be";
  if (range->compare(value, range->min) < 0 && range->isMinIncluded) {
    handleError("%s %s greater or equal to min.", range->valueName, modal);
    return LESS;
  }

  if (range->compare(value, range->min) <= 0 && !range->isMinIncluded) {
    handleError("%s %s greater than min.", range->valueName, modal);
    return LESS_EQUAL;
  }

  if (range->compare(value, range->max) > 0 && range->isMaxIncluded) {
    handleError("%s %s less or equal to max.", range->valueName, modal);
    return GREATER;
  }

  if (range->compare(value, range->max) >= 0 && !range->isMaxIncluded) {
    handleError("%s %s less than max.", range->valueName, modal);
    return GREATER_EQUAL;
  }

  return WITHIN_RANGE;
}

void printNumber(const void *value, NumberType type) {
  switch (type) {
  case TYPE_SIZE_T:
    printf("%zu", *(size_t *)value);
    break;
  case TYPE_DOUBLE:
    printf("%.*lg", DBL_DIG, *(double *)value);
    break;
  default:
    handleError("Unknown type");
    break;
  }
}

void printNumberRange(const NumberRange *range) {
  printf("from ");
  printNumber(range->min, range->type);
  printf(" to ");
  printNumber(range->max, range->type);
}

int getMaxCharCountNumber(NumberType type) {
  switch (type) {
  case TYPE_SIZE_T:
    return snprintf(NULL, 0, "%zu", SIZE_MAX);
  case TYPE_DOUBLE:
    return snprintf(NULL, 0, "%.*e", DBL_DIG, DBL_MAX);
  default:
    handleError("No such type found, aborting.");
    return 0;
  }
}

void printCharacterCountRangeNumber(NumberType type) {
  int max = getMaxCharCountNumber(type);
  if (max <= 0) {
    handleError("Failed to find max character count for type");
    return;
  }
  printf("character count for %s ≤ %u", typeDescriptions[type], max);
}

int convertInputToNumber(const char *input, void *value, NumberType type) {
  char *endptr = NULL;
  errno = 0;

  switch (type) {
  case TYPE_SIZE_T: {
    unsigned long long temp = 0;

    if (sizeof(size_t) == sizeof(unsigned long)) {
      temp = strtoul(input, &endptr, 10);
    } else if (sizeof(size_t) == sizeof(unsigned long long)) {
      temp = strtoull(input, &endptr, 10);
    } else if (sizeof(size_t) == sizeof(unsigned int)) {
      temp = strtol(input, &endptr, 10);
    } else {
      handleError("Unsupported size_t size");
      return FAILURE;
    }

    if (errno == ERANGE || temp > SIZE_MAX) {
      handleErrorOverflow();
      return FAILURE;
    }

    *(size_t *)value = (size_t)temp;

    break;
  }

  case TYPE_DOUBLE: {
    double temp = strtod(input, &endptr);
    if (errno == ERANGE) {
      handleErrorOverflow();
      return FAILURE;
    }
    *(double *)value = temp;
    break;
  }

  default:
    handleError("Unsupported number type");
    return FAILURE;
  }

  // Check if there are any non-number characters left after the number
  if (*endptr != '\0') {
    handleErrorNotNumber();
    return FAILURE;
  }

  return SUCCESS;
}

int readNumber(void *value, NumberType type) {
  int maxCharCount = getMaxCharCountNumber(type);
  if (maxCharCount <= 0) {
    handleError("Failed to find max character count for type");
    return FAILURE;
  }
  char *input = NULL;

  int result = readLine(&input, DEFAULT_STRING_LENGTH, maxCharCount, stdin);

  if (result != SUCCESS)
    return result;

  if (!input)
    return FAILURE;

  if (convertInputToNumber(input, value, type) == FAILURE) {
    free(input);
    input = NULL;
    return FAILURE;
  }

  free(input);
  input = NULL;
  return SUCCESS;
}

int readNumberWithinRange(void *value, NumberType type,
                          const NumberRange *range, const char *prompt) {
  printf("%s (", prompt);
  printNumberRange(range);
  printf("): ");

  int result = readNumber(value, type);
  switch (result) {
  case FAILURE:
    return FAILURE;
  case EOF:
    return EOF;
  }

  if (validateNumberRange(value, range) != WITHIN_RANGE)
    return FAILURE;

  return SUCCESS;
}

int readNumberWithValidation(void *value, NumberType type, const char *prompt,
                             ValidationFunc additionalCheck, ...) {
  bool isValid = false;

  va_list args;

  while (!isValid) {
    printf("%s: ", prompt);
    int result = readNumber(value, type);
    if (result == SUCCESS) {
      if (additionalCheck != NULL) {
        va_start(args, additionalCheck);
        isValid = additionalCheck(value, args);
        va_end(args);
      } else {
        isValid = true;
      }
    } else if (result == EOF) {
      return EOF;
    }
  }

  return SUCCESS;
}

int readNumberWithinRangeWithValidation(void *value, NumberType type,
                                        const NumberRange *range,
                                        const char *prompt,
                                        ValidationFunc additionalCheck, ...) {
  bool isValid = false;

  va_list args;

  while (!isValid) {
    int result = readNumberWithinRange(value, type, range, prompt);
    if (result == SUCCESS) {
      if (additionalCheck != NULL) {
        va_start(args, additionalCheck);
        isValid = additionalCheck(value, args);
        va_end(args);
      } else {
        isValid = true;
      }
    } else if (result == EOF) {
      return EOF;
    }
  }

  return SUCCESS;
}
