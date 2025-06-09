#include "../common/types.h"
#include "int.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>

void showChoices(const char **choices, int choice_count) {
  for (int i = 0; i < choice_count; i++) {
    printf("%d. %s\n", i + 1, choices[i]);
  }
}

int getUserChoice(const char *prompt, const char *choices[], int choice_count,
                  const IntRange *range) {
  printf("\n");
  showPrompt(prompt);
  printf("\n");
  showChoices(choices, choice_count);
  printf("\n");

  int choice = readIntWithinRangeWithValidation("Оберіть опцію", range, NULL);
  return choice - 1;
}
