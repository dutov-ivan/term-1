#ifndef EQUATIONS_H
#define EQUATIONS_H

#include "../algorithm.h"

void print_equations(Equations* eqs, int decimal_places);
void print_solutions(Solutions* sol, int decimal_places);
void handle_equation_input(Equations* eqs, DoubleRange* coef_range,
                           int decimal_places);

#endif  // !EQUATIONS_H
