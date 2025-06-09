#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

int check_float_meets_restrictions(float *value, const char *name,
                                   float max_value, float min_value,
                                   int is_max_included, int is_min_included,
                                   float tolerance) {
  if (is_min_included) {
    if (*value < min_value - tolerance) {
      display_error("%s має бути більший-рівний %g.", name, min_value);
      return 1;
    }
  } else {
    if (*value <= min_value + tolerance) {
      display_error("%s має бути більший за %g.", name, min_value);
      return 1;
    }
  }

  if (is_max_included) {
    if (*value > max_value + tolerance) {
      display_error("%s має бути менший-рівний %g.", name, max_value);
      return 1;
    }
  } else {
    if (*value >= max_value - tolerance) {
      display_error("%s має бути менший за %g.", name, max_value);
      return 1;
    }
  }

  return 0;
}

int read_float(float *value, const char *full_name, const char *short_name,
               int max_char_count, int is_restricted, float max_value,
               float min_value, int is_max_included, int is_min_included,
               float tolerance, int is_exit) {
  if (is_restricted == 1) {
    printf("%s (від %g до %g): ", full_name, min_value, max_value);
  } else {
    printf("%s: ", full_name);
  }

  char input[max_char_count + 2];
  char *endptr;

  if (!fgets(input, max_char_count + 2, stdin)) {
    display_error("Не вдалося прочитати ввід для %s.", short_name);
    return 1;
  }

  if (input[strlen(input) - 1] != '\n') {
    display_error("Довжина %s в символах має бути меншою за %u.", short_name,
                  max_char_count);
    clear_input();
    return 1;
  }

  if (is_exit && input[0] == EXIT_KEY && input[1] == '\n') {
    exit(0);
  }

  replace_commas_with_dots(input);

  *value = strtof(input, &endptr);
  if (endptr == input || *endptr != '\n') {
    display_error("%s має бути числом і не містити додаткових символів!",
                  short_name);
    return 1;
  }

  if (*value == 0.0f && errno == ERANGE) {
    display_error("%s настільки малий, що точність не може бути збережена",
                  short_name);
    return 1;
  }

  if (validate_float_input_precision(input, *value, 6, short_name) != 0) {
    if (request_repeat() != 0) {
      return 1;
    }
  }
  if (is_restricted == 1 &&
      check_float_meets_restrictions(value, short_name, max_value, min_value,
                                     is_max_included, is_min_included,
                                     tolerance) == 1) {
    return 1;
  }

  return 0;
}
