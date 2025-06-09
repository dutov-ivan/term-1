#ifndef DOUBLE_H
#define DOUBLE_H

#include "common/types.h"

void print_double_range(const char *name, const DoubleRange *range);
void print_double_character_count_range();
void print_double_precision_limit();
void print_truncated_double(double num, int decimal_places);
int read_double_within_range(double *value, const char *prompt,
                             const DoubleRange *range);
double read_double_within_range_with_validation(
    const char *prompt, const DoubleRange *range,
    DoubleValidation additional_check, ...);

#endif
