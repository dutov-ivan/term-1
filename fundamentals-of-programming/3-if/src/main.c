#include "input/input.h"
#include "root/root.h"

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

void process_arguments(int argc, char *argv[], int *is_quiet_mode,
                       int *is_repeat_mode, int *is_help_mode);
void print_help();
void print_demo();

int main(int argc, char *argv[]) {
  int is_quiet_mode, is_repeat_mode, is_help_mode;
  is_quiet_mode = is_repeat_mode = is_help_mode = 0;

  process_arguments(argc, argv, &is_quiet_mode, &is_repeat_mode, &is_help_mode);

  if (is_help_mode) {
    print_help();
    return 0;
  }

  print_demo();
  do {

    long double x, e;
    int k, decimal_places;
    x = e = k = decimal_places = 0;

    while (read_long_double(&x, "підкореневий вираз x", "x", MAX_CHARS_X, 1,
                            is_quiet_mode, MAX_VAL_X, -MAX_VAL_X, 1, 1) != 0 ||
           (x < 0 && check_long_double_meets_restrictions(&x, "x", -MIN_VAL_X,
                                                          -MAX_VAL_X, 1, 1)) ||
           (x > 0 && check_long_double_meets_restrictions(&x, "x", MAX_VAL_X,
                                                          MIN_VAL_X, 1, 1)))
      ;

    while (read_int(&k, "показник кореня k", "k", MAX_CHARS_K, 1, is_quiet_mode,
                    MAX_VAL_K, MIN_VAL_K, 1, 1) != 0 ||
           (x < 0 && k % 2 == 0 &&
            display_error("Неможливо порахувати корінь парного показнику з "
                          "від'ємного числа.")))
      ;

    printf(GREEN "За замовчуванням тип вводу точності - через точність e.\n"
                 "Якщо ви бажаєте обрати ввід через кількість знаків після "
                 "коми D, введіть 0" RESET "\n");

    while (read_precision_or_decimal_places(&e, &decimal_places, 1,
                                            is_quiet_mode, MAX_VAL_E,
                                            MIN_VAL_E) != 0)
      ;

    const long double result = approximate_kth_root(
        x, k, e, decimal_places, is_quiet_mode, 0, DEFAULT_ITERATION_LIMIT);
    if (result == INFINITY) {
      display_error(
          "Послідовність не зійшлась через неточність в розрахунках.");
    } else {
      display_success("Кінцевий результат у: %.*Lf.", decimal_places, result);
    }
  } while (is_repeat_mode);
  return 0;
}

void process_arguments(int argc, char *argv[], int *is_quiet_mode,
                       int *is_repeat_mode, int *is_help_mode) {
  for (int i = 0; i < argc; i++) {
    if (strcmp("-q", argv[i]) == 0 || strcmp("--quiet", argv[i]) == 0) {
      *is_quiet_mode = 1;
    } else if (strcmp("-r", argv[i]) == 0 || strcmp("--repeat", argv[i]) == 0) {
      *is_repeat_mode = 1;
    } else if (strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0) {
      *is_help_mode = 1;
    }
  }
}
void print_help() {
  printf("Програму можна запускати з такими параметрами:\n");
  printf("-q або --quiet: «тихий» режим (менше виводу)\n");
  printf("-r або --repeat: програма нескінченно повторюється\n");
  printf("-h або --help: вивести цю пам'ятку\n");
}

void print_demo() {
  printf("Вас вітає Find The KeX — програма для знаходження\n"
         "кореню k-го показника з x з точністю e\n");

  printf("Наші обмеження такі:\n");

  printf("\n%Lg ≤ x ≤ %Lg або %Lg ≤ x ≤ %Lg. ", MIN_VAL_X, MAX_VAL_X,
         -MAX_VAL_X, -MIN_VAL_X);
  printf("Довжина x в символах ≤ %u.\n", MAX_CHARS_X);

  printf("%d ≤ k ≤ %d. ", MIN_VAL_K, MAX_VAL_K);
  printf("Довжина k в символах ≤ %u.\n", MAX_CHARS_K);

  printf(
      "\nМожна вводити щось одне: кількість знаків після коми D або точність "
      "e.\n");
  printf("%u ≤ D ≤ %u\n", MIN_DECIMAL_PLACES, MAX_DECIMAL_PLACES);
  printf("%Lg < e < %Lg\n", MIN_VAL_E, MAX_VAL_E);
}
