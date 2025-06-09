#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// UTILS
void replaceCommasWithDots(char *string);

// OUTPUT
void clearLastLines(int count);
void showPrompt(const char *prompt);
void handleError(const char *format, ...);
void handleErrorOverlength(int maxCharCount);
void handleErrorMemoryAllocation(const char *reason);
void handleErrorOverflow();
void handleErrorNotNumber();
void warnNotPrecise(int maxSignificantDigits);
void showWarning(const char *format, ...);
void showSuccess(const char *format, ...);

// INPUT TAKING
void clearStdin();
char *readInput(int initialCharCount, int maxCharCount);
bool askQuestion(const char *format, ...);

// INPUT CHECKING
bool isInputWithinLength(const char *input);
bool isInputFloatingPoint(char *input);
bool isInputNumberAfterConversion(const char *endptr, const char *input);

#endif
