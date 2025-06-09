#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <float.h>
#include <stdbool.h>

#include "common/types.h"

typedef struct {
  int count;
  double** coefs;
  double* const_coefs;
} Equations;

typedef struct {
  int count;
  double* corrs;  // corrections
  double* roots;
} Solutions;

int initialize_equations(Equations* eqs);
int initialize_solutions(Solutions* sol);
void free_program(Equations* eqs, Solutions* sol);

void generate_equations(Equations* eqs, DoubleRange* coef_range);

bool is_convergent_row(Equations* eqs, int row);

int solve_equations(Equations* eqs, Solutions* sol, double precision);

#endif
