#include "geometry.h"
#include <stdio.h>
#include <stdlib.h>

#define INPUT_BUFFER_SIZE 256

int handle_input(double *x, double *y);

int main() {
  const double x1 = 1;
  const double y1 = 0;
  const double r = 2;
  const double k = -1;
  const double b = 3;

  double x, y;
  x = y = 0;

  while (handle_input(&x, &y) != 0) {
    printf("Виникла помилка. Спробуйте ще раз.\n\n");
  }

  if (is_in_circle(x, x1, y, y1, r)) {
    if (is_below_line(y, k, x, b)) {
      if (is_above_line(y, -k, x, -b)) {
        printf("Точка (%lf, %lf) знаходиться в ділянці.\n", x, y);
      }
    }
  } else {
    printf("Точка (%lf, %lf) НЕ знаходиться в ділянці.\n", x, y);
  }
}

int handle_input(double *x, double *y) {
  char input[INPUT_BUFFER_SIZE];
  printf("Введіть координати x та y через пробіл: ");
  if (!fgets(input, sizeof(input), stdin)) {
    printf("\nПОМИЛКА! Не вдалося зчитати вхідні дані!\n");
    return 1;
  }

  if (input[0] == '\n' || input[0] == '\0') {
    printf("\nПОМИЛКА! Введіть координату x!\n");
    return 1;
  }

  int newline_found = 0;
  for (int i = 0; i < INPUT_BUFFER_SIZE; i++) {
    if (input[i] == '\n') {
      input[i] = '\0';
      newline_found = 1;
      break;
    }
  }

  if (!newline_found) {
    printf("\nПОМИЛКА! Введення занадто довге.\n");
    return 1;
  }

  char *endptr = NULL;
  *x = strtold(input, &endptr);
  if (endptr == input) {
    printf("\nПОМИЛКА! x має бути числом і не містити "
           "додаткових символів!\n");
    return 1;
  }

  while (*endptr == ' ') {
    endptr++;
  }

  if (*endptr == '\n' || *endptr == '\0') {
    printf("\nПОМИЛКА! Введіть координату y!\n");
    return 1;
  }

  *y = strtold(endptr, &endptr);
  if (endptr == input || (*endptr != '\n' && *endptr != '\0')) {
    printf("\nПОМИЛКА! y має бути числом і не містити "
           "додаткових символів!\n");
    *y = 0;
    return 1;
  }
  return 0;
}
