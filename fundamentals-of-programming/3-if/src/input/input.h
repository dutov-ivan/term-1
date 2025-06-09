#ifndef INPUT_H
#define INPUT_H

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#define MAX_VAL_X (long double)1e15
#define MIN_VAL_X (long double)1e-15
#define MAX_CHARS_X (15 + 6 + 3) // 'e', '-', and '.'

#define MAX_VAL_K 1000000
#define MIN_VAL_K -1000000
#define MAX_CHARS_K 7

#define MIN_DECIMAL_PLACES 1
#define MAX_DECIMAL_PLACES 15
#define MAX_CHARS_DECIMAL_PLACES 2

#define MAX_VAL_E (long double)1
#define MIN_VAL_E (long double)1e-15
#define MAX_CHARS_E (15 + 3) // 'e', '-', and '.'

#define TOLERANCE (long double)1e-20

#define MAX_LONG_DOUBLE_DECIMAL_PLACES 308
#define MAX_CHARS_LONG_DOUBLE (MAX_LONG_DOUBLE_DECIMAL_PLACES + 3)
#define MAX_CHARS_INT (10 + 3)
#define MAX_SIGNIFICANT_DIGITS_LONG_DOUBLE 15

int display_error(const char *format, ...)
    __attribute__((format(printf, 1, 2)));
int display_warning(const char *format, ...)
    __attribute__((format(printf, 1, 2)));
int display_success(const char *format, ...)
    __attribute__((format(printf, 1, 2)));

void replace_commas_with_dots(char *string);
int validate_input_precision(const char *input, int max_significant_digits);

void set_nonblocking_input();
void reset_input_mode();

int read_long_double(long double *value, const char *full_name,
                     const char *short_name, int max_char_count,
                     int is_restricted, int is_quiet_mode,
                     long double max_value, long double min_value,
                     int is_max_included, int is_min_included);

int read_precision_or_decimal_places(long double *precision,
                                     int *decimal_places, int is_restricted,
                                     int is_quiet_mode,
                                     long double max_value_precision,
                                     long double min_value_precision);

int read_int(int *value, const char *full_name, const char *short_name,
             int max_char_count, int is_restricted, int is_quiet_mode,
             int max_value, int min_value, int is_max_included,
             int is_min_included);

int check_long_double_meets_restrictions(long double *value, const char *name,
                                         long double max_value,
                                         long double min_value,
                                         int is_max_included,
                                         int is_min_included);

int check_int_meets_restrictions(int *value, const char *name, int max_value,
                                 int min_value, int is_max_included,
                                 int is_min_included);

#endif
