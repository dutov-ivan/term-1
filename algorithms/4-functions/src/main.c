#include "square/square.h"
#include <stdio.h>

int main() {
  int A, B;
  printf("Початок відрізка A: ");
  scanf("%d", &A);
  printf("Кінець відрізка B: ");
  scanf("%d", &B);

  if (A > B) {
    printf("ПОМИЛКА: A має бути меншим або рівним B.\n");
    return 1;
  }

  int result = 0;
  for (int i = A; i <= B; i++) {
    if (is_perfect_square(i))
      result++;
  }

  printf("Кількість повних квадратів на [A; B]: %d\n", result);

  return 0;
}
