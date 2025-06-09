#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/constants.h"
#include "common/types.h"
#include "int.h"
#include "utils.h"

#define TOLERANCE DBL_EPSILON
#define RELATIVE_TOLERANCE 1e-12
#define MIN_ABSOLUTE_VALUE DBL_MIN
#define MAX_ABSOLUTE_VALUE DBL_MAX
#define MAX_CHAR_COUNT_DOUBLE 23
#define OVERFLOW_ABSOLUTE_VALUE HUGE_VAL
#define MAX_SIGNIFICANT_DIGITS DBL_DIG

const IntRange DOUBLE_CHAR_COUNT_RANGE = {1, MAX_CHAR_COUNT_DOUBLE, true, true};

// OUTPUT
void show_prompt_double_within_range(const char *prompt,
                                     const DoubleRange *range) {
  printf("%s (від %lg до %lg): ", prompt, range->min, range->max);
}

void show_range_error_double(RangeCheckResult result,
                             const DoubleRange *range) {
  switch (result) {
    case LESS:
      handle_error("Значення має бути ≥ %lg.", range->min);
      break;
    case LESS_EQUAL:
      handle_error("Значення має бути > %lg.", range->min);
      break;
    case GREATER:
      handle_error("Значення має бути ≤ %lg.", range->max);
      break;
    case GREATER_EQUAL:
      handle_error("Значення має бути < %lg.", range->max);
      break;
    default:
      break;
  }
}

// RANGES
RangeCheckResult validate_range_double(double value, const DoubleRange *range) {
  if (range->is_min_included && value < range->min - TOLERANCE) return LESS;
  if (!range->is_min_included && value <= range->min + TOLERANCE)
    return LESS_EQUAL;
  if (range->is_max_included && value > range->max + TOLERANCE) return GREATER;
  if (!range->is_max_included && value >= range->max - TOLERANCE)
    return GREATER_EQUAL;
  return WITHIN_RANGE;
}

void print_double_range(const char *name, const DoubleRange *range) {
  printf("%g", range->min);
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
  printf("%g\n", range->max);
}

void print_double_character_count_range() {
  print_int_range("кількість символів у дійсних числах",
                  &DOUBLE_CHAR_COUNT_RANGE);
}

void print_double_precision_limit() {
  printf("Будь-які дійсні числа приймаються з точністю, не меншою за %g",
         TOLERANCE);
}

RangeCheckResult validate_overflow_double(double value) {
  if (errno == ERANGE) {
    if (fabs(value) == OVERFLOW_ABSOLUTE_VALUE) return GREATER_EQUAL;
    if (fabs(value) < MIN_ABSOLUTE_VALUE) return LESS_EQUAL;
  }
  return WITHIN_RANGE;
}

// UTILS
double truncate_to_precision_double(double num, int decimal_places) {
  double factor = pow(10, decimal_places);
  return trunc(num * factor) / factor;
}

void print_truncated_double(double num, int decimal_places) {
  printf("%.*lf", decimal_places,
         truncate_to_precision_double(num, decimal_places));
}

bool is_double_precise(double value) {
  if (isinf(value) || isnan(value)) {
    return false;
  }

  if (value == 0.0) {
    return true;
  }

  double scale = pow(10, MAX_SIGNIFICANT_DIGITS);
  double scaled_value = round(value * scale);
  double rounded_value = scaled_value / scale;

  double tolerance = fabs(value) * RELATIVE_TOLERANCE;

  return fabs(value - rounded_value) < tolerance;
}

// INPUT

int read_double_quiet(double *value, const char *prompt) {
  char input[MAX_CHAR_COUNT_DOUBLE + 2];
  errno = 0;

  if (read_input(input, MAX_CHAR_COUNT_DOUBLE) == FAILURE) {
    return FAILURE;
  }

  if (!is_input_within_length(input)) {
    clear_stdin();
    handle_error_overlength(MAX_CHAR_COUNT_DOUBLE);
    return FAILURE;
  }

  replace_commas_with_dots(input);

  char *endptr;
  *value = strtod(input, &endptr);

  if (!is_input_number_after_conversion(endptr, input)) {
    handle_error_not_number();
    return FAILURE;
  }

  RangeCheckResult global_check = validate_overflow_double(*value);
  if (global_check != WITHIN_RANGE) {
    handle_error_overflow();
    return FAILURE;
  }

  if (!is_double_precise(*value)) {
    handle_error_not_precise(MAX_SIGNIFICANT_DIGITS);
    return FAILURE;
  }

  return SUCCESS;
}

int read_double(double *value, const char *prompt) {
  show_prompt(prompt);
  return read_double_quiet(value, prompt);
}

int read_double_within_range(double *value, const char *prompt,
                             const DoubleRange *range) {
  show_prompt_double_within_range(prompt, range);

  if (read_double_quiet(value, prompt) == FAILURE) {
    return FAILURE;
  }

  RangeCheckResult range_check = validate_range_double(*value, range);
  if (range_check != WITHIN_RANGE) {
    show_range_error_double(range_check, range);
    return FAILURE;
  }
  return SUCCESS;
}

double read_double_within_range_with_validation(
    const char *prompt, const DoubleRange *range,
    DoubleValidation additional_check, ...) {
  double value;
  bool is_valid = false;

  va_list args;

  while (!is_valid) {
    if (read_double_within_range(&value, prompt, range) == SUCCESS) {
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
