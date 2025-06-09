#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// UTILS
void replace_commas_with_dots(char *string);

// OUTPUT
void clear_last_lines(int count);
void show_prompt(const char *prompt);
void handle_error(const char *format, ...);
void handle_error_overlength(int max_char_count);
void handle_error_memory_allocation(const char *reason);
void handle_error_overflow();
void handle_error_not_number();
void handle_error_not_precise(int max_significant_digits);
void show_warning(const char *format, ...);
void show_success(const char *format, ...);

// INPUT TAKING
void clear_stdin();
int read_input(char *input, int max_char_count);
bool ask_question(const char *format, ...);

// INPUT CHECKING
bool is_input_within_length(const char *input);
bool is_input_floating_point(char *input);
bool is_input_number_after_conversion(const char *endptr, const char *input);

#endif
