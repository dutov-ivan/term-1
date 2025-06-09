#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/constants.h"
#include "common/types.h"
#include "utils.h"

#define MAX_VALUE (long)INT_MAX
#define MIN_VALUE (long)INT_MIN
#define MAX_CHAR_COUNT_INT 10  // TODO: make it a little bit more usable

const IntRange INT_CHAR_COUNT_RANGE = {1, MAX_CHAR_COUNT_INT, true, true};

void show_prompt_within_range(const char *prompt, const IntRange *range) {
  printf("%s (від %d до %d): ", prompt, range->min, range->max);
}

void show_range_error_int(RangeCheckResult result, const IntRange *range) {
  switch (result) {
    case LESS:
      handle_error("Значення має бути ≥ %d.", range->min);
      break;
    case LESS_EQUAL:
      handle_error("Значення має бути > %d.", range->min);
      break;
    case GREATER:
      handle_error("Значення має бути ≤ %d.", range->max);
      break;
    case GREATER_EQUAL:
      handle_error("Значення має бути < %d.", range->max);
      break;
    default:
      break;
  }
}

// Range Checking Functions
RangeCheckResult validate_range_int(int value, const IntRange *range) {
  if (range->is_min_included && value < range->min) return LESS;
  if (!range->is_min_included && value <= range->min) return LESS_EQUAL;
  if (range->is_max_included && value > range->max) return GREATER;
  if (!range->is_max_included && value >= range->max) return GREATER_EQUAL;
  return WITHIN_RANGE;
}

void print_int_range(const char *name, const IntRange *range) {
  printf("%d", range->min);
  if (range->is_min_included) {
    printf(" ≤ ");
  } else {
    printf(" < ");
  }

  printf("%s", name);

  if (range->is_max_included) {
    printf(" ≤ ");
  } else {
    printf(" < ");
  }

  printf("%d\n", range->max);
}

void print_int_character_count_range() {
  print_int_range("кількість символів у цілих числах", &INT_CHAR_COUNT_RANGE);
}

RangeCheckResult validate_overflow_int(long int value) {
  if (value > MAX_VALUE) return GREATER;
  if (value < MIN_VALUE) return LESS;
  return WITHIN_RANGE;
}

int read_int_quiet(int *value, const char *prompt) {
  char input[MAX_CHAR_COUNT_INT + 2];

  if (read_input(input, MAX_CHAR_COUNT_INT) == FAILURE) {
    return FAILURE;
  }

  if (!is_input_within_length(input)) {
    handle_error_overlength(MAX_CHAR_COUNT_INT);
    clear_stdin();
    return FAILURE;
  }

  char *endptr;
  long int temp_value = strtol(input, &endptr, 10);

  if (!is_input_number_after_conversion(endptr, input)) {
    handle_error_not_number();
    return FAILURE;
  }

  RangeCheckResult global_check = validate_overflow_int(temp_value);
  if (global_check != WITHIN_RANGE) {
    handle_error_overflow();
    return FAILURE;
  }

  *value = (int)temp_value;

  return SUCCESS;
}

int read_int(int *value, const char *prompt) {
  show_prompt(prompt);
  return read_int_quiet(value, prompt);
}

int read_int_within_range(int *value, const char *prompt,
                          const IntRange *range) {
  show_prompt_within_range(prompt, range);
  if (read_int_quiet(value, prompt) == FAILURE) {
    return FAILURE;
  };

  RangeCheckResult range_check = validate_range_int(*value, range);
  if (range_check != WITHIN_RANGE) {
    show_range_error_int(range_check, range);
    return FAILURE;
  }
  return SUCCESS;
}

int read_int_within_range_with_validation(const char *prompt,
                                          const IntRange *range,
                                          IntValidation additional_check, ...) {
  int value;
  bool is_valid = false;

  va_list args;

  while (!is_valid) {
    if (read_int_within_range(&value, prompt, range) == SUCCESS) {
      if (additional_check != NULL) {
        va_start(args, additional_check);
        is_valid = additional_check(value, args);
        va_end(args);

      } else {
        is_valid = true;
      }
    }
  }

  return value;
}
