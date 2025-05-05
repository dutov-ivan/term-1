#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "constants.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------- Function Declarations ---------------

// Triangle property calculation functions
long double get_triangle_perimeter(long double a, long double b, long double c);
long double get_triangle_area(long double a, long double b, long double c);
long double get_triangle_height(long double side, long double b, long double c);
long double get_triangle_median(long double side, long double b, long double c);
long double get_triangle_bisector(long double side, long double b,
                                  long double c);

// Input functions
int read_triangle_side(long double *side, const char *side_name);
int read_decimal_places(unsigned *decimal_places);

// Precision handling and printing functions
void print_with_additional_precision(const char *text, unsigned int precision,
                                     long double value,
                                     long double sides[SIDE_COUNT]);

// Triangle validation function
int validate_triangle(long double a, long double b, long double c);

#endif // TRIANGLE_H
