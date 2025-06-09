#include "io.h"
#include <stdlib.h>

#include <stddef.h>
#include <stdio.h>

#define DEFAULT_CHOICE_PROMPT "Choose option to select"
void showChoices(const char **choices, int choice_count) {
  for (int i = 0; i < choice_count; i++) {
    printf("%d. %s\n", i + 1, choices[i]);
  }
  printf("\n");
}

size_t getUserChoice(NumberRange *choiceRange, const char *info,
                     const char *choices[]) {

  if (info)
    printf("%s:\n", info);
  if (choices)
    showChoices(choices, *(int *)choiceRange->max);

  size_t start = 1;
  size_t end = *(size_t *)choiceRange->max;

  size_t choice;
  if (readNumberWithinRangeWithValidation(&choice, TYPE_SIZE_T, choiceRange,
                                          DEFAULT_CHOICE_PROMPT, NULL) == EOF) {
    return 0;
  }

  return choice;
}

NumberRange *initChoiceRange(size_t start, size_t end) {
  NumberRange *range =
      initializeNumberRange(TYPE_SIZE_T, &start, &end, true, true, "choice");
  if (!range)
    return NULL;
  return range;
}
