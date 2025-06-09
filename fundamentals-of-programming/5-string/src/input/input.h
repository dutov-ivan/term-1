#ifndef INPUT_INTERNALS_H
#define INPUT_INTERNALS_H

#include <stdbool.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef FAILURE
#define FAILURE 1
#endif

#ifndef PIPE
#define PIPE 1
#endif

typedef enum {
  GREATER_EQUAL,
  GREATER,
  LESS,
  LESS_EQUAL,
  WITHIN_RANGE
} RangeCheckResult;

int show_error(const char *format, ...) __attribute__((format(printf, 1, 2)));
int show_warning(const char *format, ...) __attribute__((format(printf, 1, 2)));
int show_success(const char *format, ...) __attribute__((format(printf, 1, 2)));
bool ask_question(const char *format, ...)
    __attribute__((format(printf, 1, 2)));

int show_error_overlength(const char *name, int max_char_count);
int show_error_memory_allocation(const char *reason);
int show_error_not_number(const char *name);

int read_input(char *input, int max_char_count, const char *name);

// Utils
void replace_commas_with_dots(char *string);
int ask_confirmation();
void clear_stdin();
void remove_newline(char *string);

bool is_input_floating_point(char *str);
bool is_input_within_length(const char *input);
bool is_input_number_after_conversion(const char *endptr, const char *input);

// Reading valid input
int read_int_and_validate(int *value, const char *full_name,
                          const char *short_name, int max_char_count,
                          bool is_restricted, int max_value, int min_value,
                          bool is_max_included, bool is_min_included);

int read_string_within_length(char **value, const char *name,
                              int max_char_count, bool is_numbered, int number);
#endif
