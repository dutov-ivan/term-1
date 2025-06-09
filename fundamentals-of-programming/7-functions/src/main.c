#include "main.h"

#include "common/constants.h"
#include "io/choices.h"
#include "io/double.h"
#include "io/utils.h"
#include "io/validators.h"

#include <Block.h>
#include <math.h>
#include <stdio.h>
#include "algorithm.h"

void print_demo() {
  printf("Вас вітає програма для знаходження коренів двох рівнянь:\n");
  showChoices(EQUATION_DESCRIPTIONS, EQUATION_COUNT);

  printf("\nза допомогою двох методів:\n");
  showChoices(METHOD_DESCRIPTIONS, METHOD_COUNT);
  printf("на дійсному проміжку [a, b]\n");

  showWarning("Приймаються тільки проміжки, що не містять точок розриву "
              "заданих функцій.");

  showWarning("Наші обмеження:");
  printDoubleRange("Нижня межа a, верхня межа b, параметр y", &DOUBLE_RANGE);
  printDoubleRange("Точність", &PRECISION_RANGE);
  printDoublePrecisionLimit();
  printDoubleCharacterCountRange();
  printf("\n");
}

int main() {
  print_demo();
  do {
    // First reading function because input boundaries depend on it
    int funcIndex = getUserChoice(EQUATION_PROMPT, EQUATION_DESCRIPTIONS,
                                  EQUATION_COUNT, &EQUATION_CHOICE_RANGE);

    EquationTemplate template = EQUATION_FUNCTION_TEMPLATES[funcIndex];

    double start, end;

    switch (funcIndex) {
    case YX_INDEX:
      start = readDoubleWithinRangeWithValidation("Нижня межа а", &DOUBLE_RANGE,
                                                  isNotZero);
      end = readDoubleWithinRangeWithValidation(
          "Верхня межа b", &DOUBLE_RANGE, isGreaterAndNotContainZero, start);
      break;
    case YLNX_INDEX:
      start = readDoubleWithinRangeWithValidation("Нижня межа а", &YLNX_RANGE,
                                                  NULL);
      end = readDoubleWithinRangeWithValidation("Верхня межа b", &YLNX_RANGE,
                                                isGreater, start);
      break;
    default:
      handleError("Не існує такої функції!");
      return SUCCESS;
    }

    double y =
        readDoubleWithinRangeWithValidation("Параметр y", &DOUBLE_RANGE, NULL);
    double precision = readDoubleWithinRangeWithValidation(
        "Точність e", &PRECISION_RANGE, NULL);

    EquationFunc func = template(y);
    int decimalPlaces = -(int)log10(precision);
    double x;

    do {
      int methodIndex = getUserChoice(METHOD_PROMPT, METHOD_DESCRIPTIONS,
                                      METHOD_COUNT, &METHOD_CHOICE_RANGE);

      EquationMethod method = METHODS[methodIndex];
      x = method(start, end, func, precision);

      if (x != INFINITY) { // INFINITY means error
        showSuccess("x = %.*lf", decimalPlaces, x);
        printf(EQUATION_TEMPLATES[funcIndex], y, decimalPlaces, x,
               decimalPlaces, x, decimalPlaces, x);
        printf("= %.*lg", decimalPlaces, func(x));
      }
    } while (
        x == INFINITY &&
        askQuestion("Метод не знайшов розв'язків на цьому "
                    "проміжку, чи не бажаєте ви використати інший метод?"));
    Block_release(func);
  } while (askQuestion("Чи бажаєте Ви повторити програму?"));
  return SUCCESS;
}
