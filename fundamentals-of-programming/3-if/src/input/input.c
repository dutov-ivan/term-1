#include "input.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replace_commas_with_dots(char *string) {
  while (*string) {
    if (*string == ',') {
      *string = '.';
    }
    string++;
  }
}

int validate_input_precision(const char *input, int max_significant_digits) {
  int significant_digits = 0;

  for (size_t i = 0; i < strlen(input); i++) {
    char c = input[i];

    if (c >= '0' && c <= '9') {
      significant_digits++;
    }
  }

  if (significant_digits > max_significant_digits) {
    display_warning("Ввід перевищує максимальну дозволену кількість значущих "
                    "цифр %d. Розрахунки можуть бути неточними",
                    max_significant_digits);
    return 1;
  }

  return 0;
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

int check_long_double_meets_restrictions(long double *value, const char *name,
                                         long double max_value,
                                         long double min_value,
                                         int is_max_included,
                                         int is_min_included) {

  if (is_min_included) {
    if (*value < min_value - TOLERANCE) {
      display_error("%s має бути більший-рівний %Lg.", name, min_value);
      return 1;
    }
  } else {
    if (*value <= min_value + TOLERANCE) {
      display_error("%s має бути більший за %Lg.", name, min_value);
      return 1;
    }
  }

  if (is_max_included) {
    if (*value > max_value + TOLERANCE) {
      display_error("%s має бути менший-рівний %Lg.", name, max_value);
      return 1;
    }
  } else {
    if (*value >= max_value - TOLERANCE) {
      display_error("%s має бути менший за %Lg.", name, max_value);
      return 1;
    }
  }

  return 0;
}

int read_long_double(long double *value, const char *full_name,
                     const char *short_name, int max_char_count,
                     int is_restricted, int is_quiet_mode,
                     long double max_value, long double min_value,
                     int is_max_included, int is_min_included) {
  if (is_quiet_mode == 1) {
    printf("%s: ", short_name);
  } else if (is_restricted == 1) {
    printf("Введіть %s (від %Lg до %Lg): ", full_name, min_value, max_value);
  } else {
    printf("Введіть %s: ", full_name);
  }

  char input[max_char_count + 2];
  char *endptr;

  if (!fgets(input, max_char_count + 2, stdin)) {
    display_error("Не вдалося прочитати ввід для %s.", full_name);
    return 1;
  }

  if (input[strlen(input) - 1] != '\n') {
    display_error("Довжина %s в символах має бути меншою за %u.", short_name,
                  max_char_count);
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
      ;
    return 1;
  }

  replace_commas_with_dots(input);

  *value = strtold(input, &endptr);
  if (endptr == input || *endptr != '\n') {
    display_error("%s має бути числом і не містити додаткових символів!",
                  short_name);
    return 1;
  }

  if (is_restricted == 1 && check_long_double_meets_restrictions(
                                value, short_name, max_value, min_value,
                                is_max_included, is_min_included) == 1) {
    return 1;
  }

  validate_input_precision(input, MAX_SIGNIFICANT_DIGITS_LONG_DOUBLE);

  return 0;
}

int read_precision_or_decimal_places(long double *precision,
                                     int *decimal_places, int is_restricted,
                                     int is_quiet_mode,
                                     long double max_value_precision,
                                     long double min_value_precision) {
  while (read_long_double(precision, "точність e", "e", MAX_CHARS_E, 0,
                          is_quiet_mode, 0, 0, 0, 0) != 0)
    ;

  if (*precision == 0) {
    while (read_int(decimal_places, "кількість знаків після коми D", "D",
                    MAX_CHARS_DECIMAL_PLACES, is_restricted, is_quiet_mode,
                    MAX_DECIMAL_PLACES, MIN_DECIMAL_PLACES, 1, 1) != 0)
      ;
    *precision = powl(10.0L, (long double)(-*decimal_places));
    return 0;
  }

  if (is_restricted) {
    if (check_long_double_meets_restrictions(precision, "e",
                                             max_value_precision,
                                             min_value_precision, 0, 1) != 0) {
      return 1;
    }
  }

  *decimal_places = (int)-log10l(*precision);
  return 0;
}

int check_int_meets_restrictions(int *value, const char *name, int max_value,
                                 int min_value, int is_max_included,
                                 int is_min_included) {

  if (is_min_included) {
    if (*value < min_value) {
      display_error("%s має бути більший-рівний %d.", name, min_value);
      return 1;
    }
  } else {
    if (*value <= min_value) {
      display_error("%s має бути більший за %d.", name, min_value);
      return 1;
    }
  }

  if (is_max_included) {
    if (*value > max_value) {
      display_error("%s має бути менший-рівний %d.", name, max_value);
      return 1;
    }
  } else {
    if (*value >= max_value) {
      display_error("%s має бути менший за %d.", name, max_value);
      return 1;
    }
  }

  return 0;
}

int read_int(int *value, const char *full_name, const char *short_name,
             int max_char_count, int is_restricted, int is_quiet_mode,
             int max_value, int min_value, int is_max_included,
             int is_min_included) {

  if (is_quiet_mode == 1) {
    printf("%s: ", short_name);
  } else if (is_restricted == 1) {
    printf("Введіть %s (від %d до %d): ", full_name, min_value, max_value);
  } else {
    printf("Введіть %s: ", full_name);
  }

  char input[max_char_count + 2];
  char *endptr;

  if (!fgets(input, max_char_count + 2, stdin)) {
    display_error("Не вдалося прочитати ввід для %s.\n", full_name);
    return 1;
  }
  if (input[strlen(input) - 1] != '\n') {
    display_error("Довжина %s в символах має бути меншою за %d.\n", short_name,
                  max_char_count);
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
      ;
    return 1;
  }

  replace_commas_with_dots(input);

  long double input_num = strtold(input, &endptr);
  if (endptr == input || *endptr != '\n') {
    display_error("%s має бути числом і не містити додаткових символів!\n",
                  short_name);
    return 1;
  }

  if (floor(input_num) != input_num) {
    display_error("%s має бути цілим числом.\n", short_name);
    return 1;
  }

  *value = (int)input_num;

  if (is_restricted == 1 &&
      check_int_meets_restrictions(value, short_name, max_value, min_value,
                                   is_max_included, is_min_included)) {
    return 1;
  }

  return 0;
}
