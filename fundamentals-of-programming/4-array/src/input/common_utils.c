#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "input.h"

void replace_commas_with_dots(char *string) {
  while (*string) {
    if (*string == ',') {
      *string = '.';
    }
    string++;
  }
}

int validate_float_input_precision(const char *input, float value,
                                   int max_significant_digits,
                                   const char *short_name) {
  if (value < 1e-6f && value > -1e-6f && value != 0) {
    display_warning(
        "%s має значення з більш ніж 6 знаками після коми. "
        "Точність може бути втрачена.",
        short_name);
    return 1;
  }

  char *dot = strchr(input, '.');
  if (dot != NULL) {
    int decimal_places = 0;
    for (char *p = dot + 1; *p != '\0' && *p != '\n' && *p != 'e' && *p != 'E';
         p++) {
      if (*p >= '0' && *p <= '9') {
        decimal_places++;
      }
    }
    if (decimal_places > max_significant_digits) {
      display_warning(
          "%s має значення з більш ніж %d знаками після коми. "
          "Точність може бути втрачена.",
          short_name, max_significant_digits);
      return 1;
    }
  }
  return 0;
}

void clear_input() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

int request_repeat() {
  char choice[3];
  printf(
      "Продовжити? Введіть '+' для продовження або будь-яку іншу клавішу, якщо "
      "не погоджуєтесь: ");

  if (!fgets(choice, sizeof(choice), stdin)) {
    printf("Помилка читання вводу.\n");
    return 1;
  }

  if (choice[0] == '+' && choice[1] == '\n') {
    return 0;
  }

  clear_input();

  return 1;
}

int display_error(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" RED "ПОМИЛКА! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
  return 1;
}

int display_warning(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" YELLOW "УВАГА! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
  return 1;
}

int display_success(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("\n" GREEN "ПЕРЕМОГА! ");
  vprintf(format, args);
  printf(RESET "\n");

  va_end(args);
  return 1;
}
