#include "algorithm.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "common/constants.h"
#include "io/utils.h"

const DoubleRange DIAGONAL_RANGE = {0.0, 100.0};

void initialize_generator() { srand(time(NULL)); }

int initialize_equations(Equations* eqs) {
  eqs->coefs = (double**)malloc(eqs->count * sizeof(double*));

  if (eqs->coefs == NULL) {
    free(eqs->coefs);
    handle_error_memory_allocation("вказівників на коефіцієнти рівнянь");
    return FAILURE;
  };

  for (int i = 0; i < eqs->count; i++) {
    eqs->coefs[i] = (double*)malloc(eqs->count * sizeof(double));
    if (eqs->coefs[i] == NULL) {
      for (int j = i; j >= 0; j--) {
        free(eqs->coefs[j]);
      }
      free(eqs->coefs);
      handle_error_memory_allocation("коефіцієнтів рівнянь");
    }
  }

  eqs->const_coefs = (double*)malloc(eqs->count * sizeof(double));
  if (eqs->const_coefs == NULL) {
    free(eqs->const_coefs);
    free(eqs->coefs);
    handle_error_memory_allocation("вільних членів рівнянь");
    return FAILURE;
  }
  return SUCCESS;
}

int initialize_solutions(Solutions* sol) {
  sol->roots = (double*)malloc(sol->count * sizeof(double));
  if (sol->roots == NULL) {
    free(sol->roots);
    handle_error_memory_allocation("коренів рівнянь");
    return FAILURE;
  }

  sol->corrs = (double*)malloc(sol->count * sizeof(double));
  if (sol->corrs == NULL) {
    free(sol->roots);
    free(sol->corrs);
    handle_error_memory_allocation(
        "коррекцій коренів (використовуються всередині алгоритму)");
    return FAILURE;
  }
  return SUCCESS;
}

void free_program(Equations* eqs, Solutions* sol) {
  for (int i = 0; i < eqs->count; i++) {
    free(eqs->coefs[i]);
  }
  free(eqs->coefs);
  free(eqs->const_coefs);

  free(sol->roots);
  free(sol->corrs);
}

double generate_double(const DoubleRange* range) {
  double random_0_1 = (double)rand() / ((double)RAND_MAX + 1.0);
  return (range->min + (range->max - range->min) * random_0_1);
}

void generate_equations(Equations* eqs, DoubleRange* coef_range) {
  initialize_generator();

  for (int i = 0; i < eqs->count; i++) {
    double compound_sum = 0.0;

    for (int j = 0; j < eqs->count; j++) {
      if (j == i) continue;
      eqs->coefs[i][j] = fabs(generate_double(coef_range));
      compound_sum += eqs->coefs[i][j];
    }

    // There would be no overflow for our constraints
    double diagonal_item = compound_sum + generate_double(&DIAGONAL_RANGE);
    eqs->coefs[i][i] = diagonal_item;

    eqs->const_coefs[i] = generate_double(coef_range);
  }
}

bool is_convergent_row(Equations* eqs, int row) {
  double non_diagonal_sum = 0;

  for (int i = 0; i < eqs->count; i++) {
    if (i == row) continue;
    non_diagonal_sum += fabs(eqs->coefs[row][i]);
  }

  if (fabs(eqs->coefs[row][row]) <= non_diagonal_sum) {
    return false;
  }
  return true;
}

bool is_convergent_equations(Equations* eqs) {
  for (int i = 0; i < eqs->count; i++) {
    if (!is_convergent_row(eqs, i)) {
      return false;
    }
  }
  return true;
}

int solve_equations(Equations* eqs, Solutions* sol, double precision) {
  // Ensure diagonal dominance for convergence
  if (!is_convergent_equations(eqs)) {
    return FAILURE;
  }

  for (int i = 0; i < eqs->count; i++) {
    sol->roots[i] = eqs->const_coefs[i] / eqs->coefs[i][i];  // Initial guess
  }

  double max_correction;
  do {
    max_correction = 0.0;
    for (int i = 0; i < eqs->count; i++) {
      double compound_sum = 0.0;

      for (int j = 0; j < eqs->count; j++) {
        if (j == i) continue;
        compound_sum += eqs->coefs[i][j] * sol->roots[j];
      }

      double new_value =
          (eqs->const_coefs[i] - compound_sum) / eqs->coefs[i][i];
      double delta = fabs(new_value - sol->roots[i]);

      if (delta > max_correction) {
        max_correction = delta;
      }

      sol->roots[i] = new_value;
    }
  } while (max_correction >= precision);

  return SUCCESS;
}
