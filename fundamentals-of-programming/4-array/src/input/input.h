#ifndef INPUT_H
#define INPUT_H

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
#define EXIT_KEY '-'

void clear_input();
int request_repeat();
int display_error(const char *format, ...)
    __attribute__((format(printf, 1, 2)));
int display_warning(const char *format, ...)
    __attribute__((format(printf, 1, 2)));
int display_success(const char *format, ...)
    __attribute__((format(printf, 1, 2)));

void replace_commas_with_dots(char *string);
int validate_float_input_precision(const char *input, float value,
                                   int max_significant_digits,
                                   const char *short_name);

void mock_input(const char *input);

int check_float_meets_restrictions(float *value, const char *name,
                                   float max_value, float min_value,
                                   int is_max_included, int is_min_included,
                                   float tolerance);

int read_float(float *value, const char *full_name, const char *short_name,
               int max_char_count, int is_restricted, float max_value,
               float min_value, int is_max_included, int is_min_included,
               float tolerance, int is_exit);
#endif
