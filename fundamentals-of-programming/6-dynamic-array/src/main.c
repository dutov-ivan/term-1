#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "algorithm.h"
#include "common/constants.h"
#include "common/types.h"
#include "io/double.h"
#include "io/equations.h"
#include "io/int.h"
#include "io/utils.h"
#include "io/validators.h"

const IntRange EQUATION_COUNT_RANGE = {2, 10000, true, true};
const DoubleRange COEF_RANGE = {-1e12, 1e12, true, true};
const DoubleRange PRECISION_RANGE = {DBL_EPSILON, 1.0, true, false};

void print_demo();

int main() {
  print_demo();
  do {
    Equations eqs;
    eqs.count = read_int_within_range_with_validation(
        "Розмір СЛАР", &EQUATION_COUNT_RANGE, NULL);
    if (initialize_equations(&eqs) == FAILURE) {
      return FAILURE;
    }

    DoubleRange user_coef_range;
    user_coef_range.min = read_double_within_range_with_validation(
        "Мінімальне значення коефіцієнтів", &COEF_RANGE, NULL);

    user_coef_range.max = read_double_within_range_with_validation(
        "Максимальне значення коефієнтів", &COEF_RANGE, is_greater,
        user_coef_range.min);
    user_coef_range.is_max_included = user_coef_range.is_min_included = true;

    double precision = read_double_within_range_with_validation(
        "Точність", &PRECISION_RANGE, NULL);

    int decimal_places = (int)(-log10(precision));

    if (ask_question("Чи бажаєте Ви уввімкнути випадковий режим?")) {
      generate_equations(&eqs, &user_coef_range);
      print_equations(&eqs, decimal_places);
    } else {
      handle_equation_input(&eqs, &user_coef_range, decimal_places);
    }

    Solutions sol;
    sol.count = eqs.count;

    if (initialize_solutions(&sol) == FAILURE) {
      return FAILURE;
    }

    if (solve_equations(&eqs, &sol, precision) == SUCCESS) {
      print_solutions(&sol, decimal_places);
    } else {
      handle_error("Ця система не є збіжною для методу!");
    };

    free_program(&eqs, &sol);

  } while (ask_question("Чи хочете ви повторити?"));

  return 0;
}

void print_demo() {
  printf(
      "\nВас вітає програма NumberWorld, що вирішує задану \n систему "
      "алгебраїчних рівнянь (СЛАР) методом Гаусса-Зейделя із заданою "
      "точністю\n");
  show_warning("Наші обмеження такі:");
  print_int_range("кількість рівнянь у системі", &EQUATION_COUNT_RANGE);
  print_double_range(
      "мінімальне та максимальне значення коефіцієнтів у рівняннях",
      &COEF_RANGE);
  printf(
      "мінімальне значення коефіцієнтів < максимальне значення коефіцієнтів\n");
  print_double_range("точність", &PRECISION_RANGE);

  print_double_character_count_range();
  print_int_character_count_range();
  print_double_precision_limit();

  printf("\n");
  show_warning(
      "Алгоритм Гаусса-Зейделя вимагає, щоб матриця була діагонально "
      "домінантною, тобто:");
  printf(
      "Модуль будь-якого діагонального елемента \n має бути більшим за суму "
      "модулів елементів того самого рядка\n");
  printf("Якщо Ви не дотримаєте цього правила, отримаєте помилку.\n");
  show_warning(
      "Випадкова генерація хоч і діє в проміжку, що зазначаєте Ви, \nпроте "
      "діагональні елементи підбираються для сходимості алгоритму.");
  printf("\n");
}
