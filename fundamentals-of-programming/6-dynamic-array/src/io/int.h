#ifndef INT_H
#define INT_H

#include "common/types.h"

void print_int_range(const char *name, const IntRange *range);
void print_int_character_count_range();
int read_int_within_range_with_validation(const char *prompt,
                                          const IntRange *range,
                                          IntValidation additional_check, ...);

#endif  // !INT_H
