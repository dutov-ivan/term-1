#ifndef IO_H
#define IO_H

#include "../common/common.h"

// NUMBER
void printNumberRange(const NumberRange *range);
int readNumberWithinRange(void *value, NumberType type,
                          const NumberRange *range, const char *prompt);
int readNumberWithinRangeWithValidation(void *value, NumberType type,
                                        const NumberRange *range,
                                        const char *prompt,
                                        ValidationFunc additionalCheck, ...);

extern char *numericTypeDescriptions[TYPE_COUNT];

NumberRange *initializeNumberRange(NumberType type, void *min, void *max,
                                   bool isMinIncluded, bool isMaxIncluded,
                                   const char *valueName);

int readNumberWithValidation(void *value, NumberType type, const char *prompt,
                             ValidationFunc additionalCheck, ...);

RangeCheckResult validateNumberRange(const void *value,
                                     const NumberRange *range);

int compareDouble(const void *a, const void *b);
int compareLongUnsigned(const void *a, const void *b);
void freeNumberRange(NumberRange *range);
void printNumber(const void *value, NumberType type);

// STRINGS
bool isLineEmpty(const char *line);
bool isValidUtf8(const char *str);
char *trimWhitespaceUtf8(char *str);
size_t getUtf8StringLength(const char *str);
size_t findCharsInUtf8String(const char *str, const char *reject);
char *readStringWithFilterUntilValid(const char *prompt, size_t maxLength,
                                     const char *reject);
bool hasSuffix(const char *filename, const char *suffix);
int compareUtf8Strings(const char *str1, const char *str2);

// CHOICES
size_t getUserChoice(NumberRange *choiceRange, const char *info,
                     const char *choices[]);

NumberRange *initChoiceRange(size_t start, size_t end);

// VALIDATORS
bool isNotZero(void *value, va_list args);
#endif
