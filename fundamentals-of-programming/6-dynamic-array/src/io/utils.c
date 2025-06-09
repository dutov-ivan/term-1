#include "utils.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/constants.h"

#define MOVE_UP "\033[F"
#define CLEAR_LINE "\033[2K"

#define ERROR_CLEAR_LINE_COUNT 4

// UTILS
void replace_commas_with_dots(char *string) {
  while (*string) {
    if (*string == ',') {
      *string = '.';
    }
    string++;
  }
}

// OUTPUT

void clear_last_lines(int count) {
  for (int i = 0; i < count; i++) {
    printf(MOVE_UP CLEAR_LINE);
  }
}

void show_prompt(const char *prompt) { printf("%s:", prompt); }

void continue_after_error() {
  printf("Натисніть Enter, щоб продовжити: ");
  fflush(stdout);
  clear_stdin();
  // clear_last_lines(ERROR_CLEAR_LINE_COUNT); avoid clearing so that it can be
  // seen in report
}

void handle_error(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" RED "ПОМИЛКА! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
  continue_after_error();
}

void handle_error_overflow() {
  handle_error("Число поза допустимими межами значень типу!");
}

void handle_error_not_precise(int max_significant_digits) {
  handle_error(
      "Кількість значущих цифр перевищує максимальну дозволену кількість цифр "
      "%d, тому розрахунки стануть неточними!",
      max_significant_digits);
}

void handle_error_overlength(int max_char_count) {
  handle_error(
      "Довжина значення в символах має бути більше 0 та меншою-рівною %d.",
      max_char_count);
}

void handle_error_memory_allocation(const char *reason) {
  handle_error("Не вдалося виділити достатньо пам'яті для %s.", reason);
}

void handle_error_not_number() {
  handle_error("Значення має бути числом і не містити додаткових символів!");
}

void show_warning(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" YELLOW "УВАГА! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
}

void show_success(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" GREEN "ПЕРЕМОГА! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
}

// INPUT TAKING
bool is_agree() {
  char choice;

  printf(
      "Введіть +, якщо погоджуєтесь. Інакше введіть будь-яку "
      "іншу клавішу: ");

  choice = getchar();
  clear_stdin();

  if (choice == '+') {
    return true;
  }

  return false;
}

void clear_stdin() {
  int c;
  while ((c = getc(stdin)) != '\n' && c != EOF);
}

int read_input(char *input, int max_char_count) {
  if (!fgets(input, max_char_count + 2, stdin)) {
    handle_error("Не вдалося прочитати ввід.");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

bool ask_question(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf("\n" YELLOW);
  vprintf(format, args);
  printf(RESET "\n");
  va_end(args);

  return is_agree();
}

// INPUT CHECKING
bool is_input_within_length(const char *input) {
  // Last character should be '\n' if input is within length
  return (input[strlen(input) - 1] == '\n' && strlen(input) - 1 != 0);
}

bool is_input_floating_point(char *input) {
  while (*input) {
    switch (*input) {
      case '.':
      case ',':
      case 'e':
      case 'E':
        return true;
      default:
        break;
    }
    input++;
  }
  return false;
}

bool is_input_number_after_conversion(const char *endptr, const char *input) {
  return (endptr != input && *endptr == '\n');
}
