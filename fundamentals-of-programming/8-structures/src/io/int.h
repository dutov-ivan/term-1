#ifndef INT_H
#define INT_H

#include "../common/types.h"
#include <stdarg.h>
#include <stdbool.h>

#define MAX_VALUE_INT (long)INT_MAX
#define MIN_VALUE_INT (long)INT_MIN
#define MAX_CHAR_COUNT_INT 10

void printIntRange(const char *name, const IntRange *range);
void printIntCharacterCountRange();
int readIntWithinRangeWithValidation(const char *prompt, const IntRange *range,
                                     IntValidation additionalCheck, ...);

#endif // !INT_H
