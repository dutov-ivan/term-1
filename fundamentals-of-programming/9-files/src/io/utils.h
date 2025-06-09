#ifndef UTILS_H
#define UTILS_H

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#include <stdbool.h>
#include <stdio.h>

bool askQuestion(const char *format, ...);
void discardTillNewline();
int readLine(char **buffer, size_t initialLength, size_t maxLength,
             FILE *stream);

void clearLastLines(int count);
int handleError(const char *format, ...);
int handleErrorMemoryAllocation(const char *reason);
int handleErrorOverflow();
int handleErrorOverlength(size_t maxCharCount);
int handleErrorNotNumber();
int showWarning(const char *format, ...);
int showSuccess(const char *format, ...);

#endif
