#ifndef UTILS_H
#define UTILS_H

#include "constants.h"
#include <stdio.h>
#include <string.h>

void flush_input_buffer();
void replace_commas_with_dots(char *str);
void truncate_to_precision(char *formatted_value, unsigned final_precision);

#endif // UTILS_H
