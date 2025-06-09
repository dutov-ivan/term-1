#include "equations.h"

#include <stdbool.h>
#include <stdio.h>

#include "../algorithm.h"
#include "common/constants.h"
#include "double.h"
#include "utils.h"

#define COEFS_PER_ROW 5
#define ROOTS_PER_ROW 5
#define CLEAR_LINE_COUNT 1

void print_equation_row(double* row_coefs, int start, int end,
                        int decimal_places) {
  for (int j = start; j <= end; j++) {
    print_truncated_double(row_coefs[j], decimal_places);
    printf("x_%d", j + 1);
    if (j < end) {
      printf(" + ");
    }
  }
}

void print_equations(Equations* eqs, int decimal_places) {
  for (int i = 0; i < eqs->count; i++) {
    printf(GREEN "Рівняння №%d:" RESET "\n", i + 1);  // Print equation header

    for (int j = 0; j < eqs->count; j += COEFS_PER_ROW) {
      bool is_last = (j + COEFS_PER_ROW) >= eqs->count;
      int row_end = is_last ? (eqs->count - 1) : (j + COEFS_PER_ROW - 1);

      if (j > 0) {
        printf("\t");
      }

      print_equation_row(eqs->coefs[i], j, row_end, decimal_places);

      if (!is_last) {
        printf(" +\n");
      }
    }

    printf(" = ");
    print_truncated_double(eqs->const_coefs[i], decimal_places);
    printf("\n");
  }
}

void print_solutions(Solutions* sol, int decimal_places) {
  show_success("Корені знайдено успішно!");
  for (int i = 0; i < sol->count; i++) {
    printf("x_%d = ", i + 1);
    print_truncated_double(sol->roots[i], decimal_places);

    if (i != sol->count - 1) {
      printf(", ");
    }

    if (i % ROOTS_PER_ROW == (ROOTS_PER_ROW - 1) &&
        sol->count > ROOTS_PER_ROW) {
      printf("\n");
    }
  }
}

void ask_for_coef(double* row_coefs, int coef_index, int decimal_places) {
  int last_row_start = coef_index - (coef_index % COEFS_PER_ROW);

  print_equation_row(row_coefs, last_row_start, coef_index - 1, decimal_places);

  if (coef_index != 0) {
    printf(" + ");
  }

  printf("? • x_%d ", coef_index + 1);
}

void handle_equation_input(Equations* eqs, DoubleRange* coef_range,
                           int decimal_places) {
  for (int i = 0; i < eqs->count; i++) {
    printf(GREEN "Введіть рівняння №%d:\n" RESET, i + 1);

    for (int coef_index = 0; coef_index < eqs->count; coef_index++) {
      ask_for_coef(eqs->coefs[i], coef_index, decimal_places);
      while (read_double_within_range(&eqs->coefs[i][coef_index], "",
                                      coef_range) == FAILURE) {
        ask_for_coef(eqs->coefs[i], coef_index, decimal_places);
      };

      bool is_row_end = (coef_index % COEFS_PER_ROW) == (COEFS_PER_ROW - 1);

      // clear_last_lines(CLEAR_LINE_COUNT); avoid clearing so that it can be
      // seen in report
      if (is_row_end) {
        int last_row_start = coef_index - (coef_index % COEFS_PER_ROW);
        print_equation_row(eqs->coefs[i], last_row_start, coef_index,
                           decimal_places);
        printf(" + \n");
      }
    }

    if (!is_convergent_row(eqs, i)) {
      int equation_line_count =
          (eqs->count + COEFS_PER_ROW - 1) / COEFS_PER_ROW;
      handle_error(
          "Введений Вами рядок коефіцієнтів не задовольняє умови для \n"
          "правильної роботи алгоритму Гаусса-Зейделя");
      clear_last_lines(equation_line_count + 1);
      i--;
      continue;
    }

    do {
      print_equation_row(eqs->coefs[i],
                         eqs->count - (eqs->count % COEFS_PER_ROW),
                         eqs->count - 1, decimal_places);
      printf(" = ?: ");
    } while (read_double_within_range(&eqs->const_coefs[i], "", coef_range) ==
             FAILURE);

    // clear_last_lines(CLEAR_LINE_COUNT); avoid clearing so that it can be seen
    // in report

    print_equation_row(eqs->coefs[i], eqs->count - (eqs->count % COEFS_PER_ROW),
                       eqs->count - 1, decimal_places);
    printf(" = ");
    print_truncated_double(eqs->const_coefs[i], decimal_places);
    printf("\n");
  }
}
