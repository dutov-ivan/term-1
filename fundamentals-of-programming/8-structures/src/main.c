#include "main.h"
#include "common/constants.h"
#include "io/choices.h"
#include "io/double.h"
#include "io/utils.h"
#include "io/validators.h"
#include <math.h>
#include <stdio.h>

void print_demo() {
  printf("Вас вітає програма SuperContur, що обчислює комплексний опір \n"
         "4 можливих коливальних контурів у залежності від частоти струму.\n");

  showWarning("Наші обмеження:");
  printDoublePrecisionLimit();
  printDoubleCharacterCountRange();
  printDoubleRange("Індукція (мГн), Ємність (мкФ), Опір (Ом), Частота (кГц)",
                   &DEFAULT_RANGE);
  printf("Мінімальна частота ≤ максимальна частота\n");
  printf("Крок частоти ≤ максимальна - мінімальна частота\n");
  printf("Крок не може бути нульовим.\n");
  showWarning("При всіх обчисленнях одиниці вимірювання не переводяться! "
              "Відповідь 100% не відповідатиме дійсності. Користуйтеся цією "
              "програмою при вимірюваннях на свій страх і ризик!");
}

double getCircuitParam(const char *prompt) {
  return readDoubleWithinRangeWithValidation(prompt, &DEFAULT_RANGE, NULL);
}

int main() {
  print_demo();
  do {
    int circuitIndex = getUserChoice(CIRCUIT_PROMPT, CIRCUIT_DESCRIPTIONS,
                                     CIRCUIT_COUNT, &CIRCUIT_CHOICE_RANGE);
    CircuitParams circuit;
    circuit.L = getCircuitParam("Індукція L (мГн)");
    circuit.C = getCircuitParam("Ємність C (мкФ)");

    if (circuitIndex == R2CLR1_INDEX || circuitIndex == R2LCR1_INDEX) {
      circuit.R1 = getCircuitParam("Опір R1 (Ом)");
      circuit.R2 = getCircuitParam("Опір R2 (Ом)");
    } else {
      circuit.R1 = getCircuitParam("Опір R (Ом)");
    }

    double f_min = getCircuitParam("Мінімальна частота f_min (кГц)");
    double f_max = readDoubleWithinRangeWithValidation(
        "Максимальна частота f_max (кГц)", &DEFAULT_RANGE, isGreaterOrEqual,
        f_min);

    DoubleRange fRange = {0, f_max - f_min, true, true};
    double df = readDoubleWithinRangeWithValidation("Крок зміни частоти (кГц)",
                                                    &fRange, NULL);

    double resonantFrequency = 1.0 / (2.0 * M_PI * sqrt(circuit.L * circuit.C));
    printf("Резонансна частота: %.*lg кГц.\n", MAX_DIGITS_DOUBLE,
           resonantFrequency);

    CircuitSolver solver = CIRCUIT_SOLVERS[circuitIndex];

    if (df == 0.0) {
      // Displaying once, cause we will be staying in place
      circuit.w = 2.0 * M_PI * f_min;
      Complex Z = solver(&circuit);
      printf("f = %.*lg кГц  Z = %.*lg + %.*lgj Ом\n", MAX_DIGITS_DOUBLE, f_min,
             MAX_DIGITS_DOUBLE, Z.Re, MAX_DIGITS_DOUBLE, Z.Im);

    } else {
      const int COUNT_PADDING = (int)log10((f_max - f_min) / df) + 1;
      int i = 1;
      double f = f_min;

      do {
        circuit.w = 2.0 * M_PI * f;
        Complex Z = solver(&circuit);
        printf("f%-*d = %-*.*lg кГц  Z%-*d = %-*.*lg + %-*.*lgj Ом\n",
               COUNT_PADDING, i, COLUMN_WIDTH, MAX_DIGITS_DOUBLE, f,
               COUNT_PADDING, i, COLUMN_WIDTH, MAX_DIGITS_DOUBLE, Z.Re,
               MAX_DIGITS_DOUBLE, COLUMN_WIDTH, Z.Im);

        f += df;
        i++;
      } while (f <= f_max + TOLERANCE_DOUBLE);
    }

  } while (askQuestion("Чи хочете Ви повторити?"));

  return SUCCESS;
}
