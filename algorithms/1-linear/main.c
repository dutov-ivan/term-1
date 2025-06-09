#include <stdio.h>

int main() {
  double a, b;
  scanf("%lf %lf", &a, &b);

  double a2 = a * a;
  double b2 = b * b;

  double sum = a2 + b2;
  double sub = a2 - b2;
  double mul = a2 * b2;
  double div = a2 / b2;

  printf("Сума квадратів = %lf\n", sum);
  printf("Різниця квадратів = %lf\n", sub);
  printf("Добуток квадратів = %lf\n", mul);
  printf("Частка квадратів = %lf\n", div);

  return 0;
}
