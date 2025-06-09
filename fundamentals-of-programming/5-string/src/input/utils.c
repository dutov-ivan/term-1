#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "input.h"

int show_error(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" RED "ПОМИЛКА! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
  return PIPE;
}

int show_warning(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" YELLOW "УВАГА! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
  return PIPE;
}

int show_success(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" GREEN "ПЕРЕМОГА! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
  return PIPE;
}

int ask_confirmation() {
  char choice;

  printf(
      "Введіть +, якщо погоджуєтесь. Інакше введіть будь-яку "
      "іншу клавішу: ");

  choice = getchar();
  clear_stdin();

  if (choice == '+') {
    return SUCCESS;
  }

  return FAILURE;
}

bool ask_question(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf("\n" YELLOW);
  vprintf(format, args);
  printf(RESET "\n");
  va_end(args);

  return (ask_confirmation() == SUCCESS) ? true : false;
}

int show_error_overlength(const char *name, int max_char_count) {
  show_error("Довжина %s в символах має бути меншою за %d.\n", name,
             max_char_count);
  return PIPE;
}

int show_error_memory_allocation(const char *reason) {
  show_error("Не вдалося виділити достатньо пам'яті для %s.", reason);
  return PIPE;
}

int show_error_not_number(const char *name) {
  show_error("%s має бути числом і не містити додаткових символів!", name);
  return PIPE;
}

int read_input(char *input, int max_char_count, const char *name) {
  if (!fgets(input, max_char_count + 2, stdin)) {
    show_error("Не вдалося прочитати ввід для %s.\n", name);
    return FAILURE;
  }
  return SUCCESS;
}

void clear_stdin() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

void remove_newline(char *string) {
  size_t string_length = strlen(string);
  if (string_length > 0 && string[string_length - 1] == '\n') {
    string[string_length - 1] = '\0';
  }
}

bool is_input_within_length(const char *input) {
  // Last character should be '\n' if input is within length
  return (input[strlen(input) - 1] == '\n' && strlen(input) - 1 != 0);
}

bool is_input_floating_point(char *str) {
  while (*str) {
    switch (*str) {
      case '.':
      case ',':
      case 'e':
      case 'E':
        return true;
      default:
        break;
    }
    str++;
  }
  return false;
}

bool is_input_number_after_conversion(const char *endptr, const char *input) {
  return (endptr != input && *endptr == '\n');
}
