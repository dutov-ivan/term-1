#include "utils.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/constants.h"

#define MOVE_UP "\033[F"
#define CLEAR_LINE "\033[2K"

#define ERROR_CLEAR_LINE_COUNT 4

// UTILS
void replaceCommasWithDots(char *string) {
  while (*string) {
    if (*string == ',') {
      *string = '.';
    }
    string++;
  }
}

// OUTPUT
void clearLastLines(int count) {
  for (int i = 0; i < count; i++) {
    printf(MOVE_UP CLEAR_LINE);
  }
}

void showPrompt(const char *prompt) { printf("%s:", prompt); }

void continueAfterError() {
  printf("Натисніть Enter, щоб продовжити: ");
  fflush(stdout);
  clearStdin();
  // NOTE: Remove this line when showcasing via screenshotting
  clearLastLines(ERROR_CLEAR_LINE_COUNT);
}

void handleError(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" RED "ПОМИЛКА! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
  continueAfterError();
}

void handleErrorOverflow() {
  handleError("Число поза допустимими межами значень типу!");
}

void warnNotPrecise(int maxSignificantDigits) {
  showWarning(
      "Кількість значущих цифр перевищує максимальну дозволену кількість цифр "
      "%d, тому розрахунки стануть неточними!",
      maxSignificantDigits);
}

void handleErrorOverlength(int maxCharCount) {
  handleError(
      "Довжина значення в символах має бути більше 0 та меншою-рівною %d.",
      maxCharCount);
}

void handleErrorMemoryAllocation(const char *reason) {
  handleError("Не вдалося виділити достатньо пам'яті для %s.", reason);
}

void handleErrorNotNumber() {
  handleError("Значення має бути числом і не містити додаткових символів!");
}

void showWarning(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" YELLOW "УВАГА! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
}

void showSuccess(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf(GREEN);
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
}

// INPUT TAKING
bool isAgree() {
  char choice;

  printf("Введіть +, якщо погоджуєтесь. Інакше введіть будь-яку "
         "іншу клавішу: ");

  choice = getchar();
  clearStdin();

  if (choice == '+') {
    return true;
  }

  return false;
}

void clearStdin() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

char *readInput(int initialCharCount, int maxCharCount) {
  int charCount = initialCharCount;
  char *input = (char *)malloc(charCount * sizeof(char));

  if (input == NULL) {
    handleErrorMemoryAllocation("рядка вводу");
  }

  bool isCompleted = false;
  int totalLength = 0;

  while (!isCompleted) {
    if (!fgets(input + totalLength, charCount - totalLength, stdin)) {
      handleError("Не вдалося прочитати ввід.");
      return NULL;
    }

    if (isInputWithinLength(input)) {
      break;
    } else if (charCount == maxCharCount) {
      clearStdin();
      handleErrorOverlength(maxCharCount);
      free(input);
      return NULL;
    }

    totalLength = strlen(input);

    charCount *= 2;

    if (charCount > maxCharCount) {
      charCount = maxCharCount;
    }

    char *newBuffer = realloc(input, charCount * sizeof(char));

    if (newBuffer == NULL) {
      handleError("Не вдалося збільшити пам'ять під ввід.");
      free(input);
      return NULL;
    }

    input = newBuffer;
  }

  return input;
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

// INPUT CHECKING
bool isInputWithinLength(const char *input) {
  // Last character should be '\n' if input is within length
  return (input[strlen(input) - 1] == '\n' && strlen(input) - 1 != 0);
}

bool isInputFloatingPoint(char *input) {
  while (*input) {
    switch (*input) {
    case '.':
    case ',':
    case 'e':
    case 'E':
      return true;
    }
    input++;
  }
  return false;
}

bool isInputNumberAfterConversion(const char *endptr, const char *input) {
  return (endptr != input && *endptr == '\n');
}
