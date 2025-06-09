#include <float.h>
#include <math.h>
#include <stdio.h>

#include "input/input.h"

#define MAX_CHARS_FLOAT 50
#define MAX_CHARS_DEGREES MAX_CHARS_FLOAT
#define MAX_CHARS_PRECISION MAX_CHARS_FLOAT

#define MAX_DEGREES FLT_MAX
#define MIN_DEGREES FLT_MIN

#define MAX_PRECISION 1.0f
#define MIN_PRECISION FLT_MIN
#define TOLERANCE 1e-7f

#define COLUMN_WIDTH 20
#define COLUMN_COUNT 4
#define BORDER_COUNT COLUMN_COUNT + 1
#define SEPARATOR '='
#define DECIMAL_PLACES 15

float mod360(float value) { return fmodf(value, 360.0f); }

float degrees_to_radians(float degrees) { return degrees / 180.0f * M_PI; }

float calculate_taylor_sin(float x, float e) {
  float d, sin_;
  d = sin_ = x;
  float n = 1.0f;
  do {
    d *= -x * x / (n + 1.0f) / (n + 2.0f);
    sin_ += d;
    n += 2.0f;
  } while (fabs(d) > e);
  return sin_;
}

void print_separator(int length) {
  for (int i = 0; i < length; i++) {
    putchar(SEPARATOR);
  }
  putchar('\n');
}

void print_calculations_row(float x, float e) {
  float rad_x = degrees_to_radians(mod360(x));
  float sin_x = sinf(rad_x);
  float taylor_sin = calculate_taylor_sin(rad_x, e);
  float diff = sin_x - taylor_sin;
  printf("|%-*.*g|%-*.*g|%-*.*g|%-*.*g|\n", COLUMN_WIDTH, DECIMAL_PLACES, x,
         COLUMN_WIDTH, DECIMAL_PLACES, sin_x, COLUMN_WIDTH, DECIMAL_PLACES,
         taylor_sin, COLUMN_WIDTH, DECIMAL_PLACES, diff);
}

void print_demo() {
  printf(
      "Ця програма знаходить sin(x) для відрізку [x1; x2] (у градусах) з \n "
      "кроком dx (у градусах) двома способами: \n вбудованою функцією та "
      "розкладом у ряд Тейлора\n");

  printf("Наші обмеження такі:\n");

  printf("\n%g ≤ x1, x2, dx ≤ %g або \n %g ≤ x1, x2, dx ≤ %g.\n", MIN_DEGREES,
         MAX_DEGREES, -MAX_DEGREES, -MIN_DEGREES);

  printf("Якщо x1 = x2, то dx = 0\n");
  printf("Якщо x1 ≠ x2, то dx ≠ 0\n");
  printf("Якщо x1 < x2, то dx > 0\n");
  printf("Якщо x1 > x2, то dx < 0\n");

  printf("\n%g ≤ e < %g\n", MIN_PRECISION, MAX_PRECISION);
  printf("Довжина x1, x2, dx в символах ≤ %u.\n", MAX_CHARS_DEGREES);
  display_warning(
      "Числа, різниця між якими менша за %g, вважаються рівними. \n"
      "Максимальна гарантована точність 1e-6\n"
      "Введіть '-' при будь-якому вводі для виходу з програми\n",
      TOLERANCE);
}

int main() {
  float x1, x2, dx, e;
  x1 = x2 = dx = e = 0;
  print_demo();

  int is_repeat;

  do {
    is_repeat = 0;
    while (read_float(&x1, "Нижня межа діапазону x1", "x1", MAX_CHARS_DEGREES,
                      1, MAX_DEGREES, -MAX_DEGREES, 1, 1, TOLERANCE, 1) != 0 ||
           (x1 > 0 &&
            check_float_meets_restrictions(&x1, "x1", MAX_DEGREES, MIN_DEGREES,
                                           1, 1, TOLERANCE) != 0) ||
           (x1 < 0 &&
            check_float_meets_restrictions(
                &x1, "x1", -MIN_DEGREES, -MAX_DEGREES, 1, 1, TOLERANCE) != 0));

    while (read_float(&x2, "Верхня межа діапазону x2", "x2", MAX_CHARS_DEGREES,
                      1, MAX_DEGREES, -MAX_DEGREES, 1, 1, TOLERANCE, 1) != 0 ||
           (x2 > 0 &&
            check_float_meets_restrictions(&x2, "x2", MAX_DEGREES, MIN_DEGREES,
                                           1, 1, TOLERANCE) != 0) ||
           (x2 < 0 &&
            check_float_meets_restrictions(
                &x2, "x2", -MIN_DEGREES, -MAX_DEGREES, 1, 1, TOLERANCE) != 0));

    while (
        read_float(&dx, "Крок зміни аргументу dx", "dx", MAX_CHARS_DEGREES, 1,
                   MAX_DEGREES, -MAX_DEGREES, 1, 1, TOLERANCE, 1) != 0 ||
        (dx > 0 &&
         check_float_meets_restrictions(&dx, "dx", MAX_DEGREES, MIN_DEGREES, 1,
                                        1, TOLERANCE) != 0) ||
        (dx < 0 &&
         check_float_meets_restrictions(&dx, "dx", -MIN_DEGREES, -MAX_DEGREES,
                                        1, 1, TOLERANCE) != 0) ||
        (x1 != x2 && dx == 0 &&
         display_error("Крок зміни аргумента не може бути нульовим, якщо "
                       "значення меж різні.")) ||
        (x1 == x2 && dx != 0 &&
         display_error("Крок зміни аргумента не може бути не 0, якщо значення "
                       "меж однакові.")) ||
        (x1 < x2 && dx < 0 &&
         display_error(
             "Крок зміни аргумента dx має бути додатнім, якщо x1 < x2.")) ||
        (x1 > x2 && dx > 0 &&
         display_error(
             "Крок зміни аргумента dx має бути від'ємним, якщо x1 > x2.")));

    while (read_float(&e, "Точність e", "e", MAX_CHARS_PRECISION, 1, 1,
                      MIN_PRECISION, 0, 1, TOLERANCE, 1) != 0);

    printf("\n");
    print_separator(COLUMN_WIDTH * COLUMN_COUNT + BORDER_COUNT);
    printf("|%-*s|%-*s|%-*s|%-*s|\n", COLUMN_WIDTH, "x", COLUMN_WIDTH, "sin(x)",
           COLUMN_WIDTH, "taylor_sin", COLUMN_WIDTH, "diff");
    print_separator(COLUMN_WIDTH * COLUMN_COUNT + BORDER_COUNT);

    int iteration_limit = (dx == 0) ? 1 : (int)fabsf((x1 - x2) / dx) + 1;
    int i = 0;

    if (x1 <= x2) {
      for (float x = x1; x <= x2 && i < iteration_limit; x += dx) {
        print_calculations_row(x, e);
        i++;
      }
    } else {
      for (float x = x1; x >= x2 && i < iteration_limit; x += dx) {
        print_calculations_row(x, e);
        i++;
      }
    }

    print_separator(COLUMN_WIDTH * COLUMN_COUNT + BORDER_COUNT);
    if (request_repeat() == 0) {
      is_repeat = 1;
    };
    print_separator(COLUMN_WIDTH * COLUMN_COUNT + BORDER_COUNT);
  } while (is_repeat);
  return 0;
}
