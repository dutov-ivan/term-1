#include "utils.h"
#include "../common/common.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// INPUT

void discardTillNewline() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

bool isAgree() {
  char choice;

  printf("Enter 'y' if you agree. Otherwise press whatever... ");

  choice = getchar();
  if (choice == '\n')
    return false;

  discardTillNewline();

  if (choice == 'y') {
    return true;
  }

  return false;
}

bool askQuestion(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf("\n" YELLOW);
  vprintf(format, args);
  printf(RESET "\n");
  va_end(args);

  return isAgree();
}

int readLine(char **buffer, size_t initialLength, size_t maxLength,
             FILE *stream) {
  if (buffer == NULL || stream == NULL || initialLength == 0) {
    handleError("Invalid arguments to readLine.");
    return FAILURE;
  }

  size_t bufferSize = initialLength;
  char *input = (char *)malloc(bufferSize * sizeof(char));
  if (input == NULL) {
    handleErrorMemoryAllocation("input buffer");
    return FAILURE;
  }

  size_t filledBufferSize = 0;
  bool isCompleted = false;

  while (!isCompleted) {
    if (!fgets(input + filledBufferSize, bufferSize - filledBufferSize,
               stream)) {
      if (feof(stream)) {
        free(input);
        return EOF;
      }

      free(input);
      handleError("Failed to read input.");
      return FAILURE;
    } else {
      filledBufferSize += strlen(input + filledBufferSize);
      if (input[filledBufferSize - 1] == '\n') {
        isCompleted = true;
        input[filledBufferSize - 1] = '\0'; // Remove newline character
      }
    }

    if (!isCompleted && filledBufferSize == bufferSize - 1) {
      size_t newBufferSize = bufferSize * 2;
      if (maxLength != INFINITE_LENGTH && newBufferSize > maxLength) {
        newBufferSize = maxLength;
      }

      if (newBufferSize <= bufferSize) {
        discardTillNewline();
        handleErrorOverlength(maxLength);
        free(input);
        return FAILURE;
      }

      char *newBuffer = realloc(input, newBufferSize * sizeof(char));
      if (newBuffer == NULL) {
        free(input);
        handleError("Failed to expand input buffer.");
        return FAILURE;
      }

      input = newBuffer;
      bufferSize = newBufferSize;
    }

    if (maxLength != INFINITE_LENGTH && filledBufferSize >= maxLength) {
      discardTillNewline();
      handleErrorOverlength(maxLength);
      free(input);
      return FAILURE;
    }
  }

  *buffer = input; // Final buffer contains the input
  return SUCCESS;
}

// OUTPUT
#include <errno.h>

#define MOVE_UP "\033[F"
#define CLEAR_LINE "\033[2K"

#define ERROR_CLEAR_LINE_COUNT 3

void clearLastLines(int count) {
  for (int i = 0; i < count; i++) {
    printf(MOVE_UP CLEAR_LINE);
  }
}

void continueAfterError() {
  printf("Press Enter to continue: ");
  fflush(stdout);
  discardTillNewline(); // Getting any input from user
  // NOTE: Remove this line when showcasing via screenshotting
  clearLastLines(ERROR_CLEAR_LINE_COUNT);
}

int handleError(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" RED "ERROR! ");
  vprintf(format, args);
  if (errno != 0) {
    perror("");
    errno = 0;
  }
  printf(RESET "\n");

  va_end(args);
  continueAfterError();
  return PIPE;
}

int handleErrorOverflow() {
  return handleError("Number is outside the allowed values of type!");
}

int warnNotPrecise(int maxSignificantDigits) {
  return showWarning(
      "The number of significant digits exceeds max allowed significant digits "
      "count %d, so some calculations may be imprecise.",
      maxSignificantDigits);
}

int handleErrorOverlength(size_t maxCharCount) {
  return handleError("The string length should be > 0 and < %llu",
                     maxCharCount);
}

int handleErrorMemoryAllocation(const char *sufferer) {
  return handleError("Failed to allocate memory for %s.", sufferer);
}

int handleErrorNotNumber() {
  return handleError(
      "Value should be a number and not contain any additional characters!");
}

int showWarning(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" YELLOW "ATTENTION! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
  return PIPE;
}

int showSuccess(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf(GREEN);
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
  return PIPE;
}
