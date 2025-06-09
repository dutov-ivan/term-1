#ifndef CHOICES_H
#define CHOICES_H

#include "../common/types.h"
void showChoices(const char **choices, int choice_count);
int getUserChoice(const char *prompt, const char *choices[], int choice_count,
                  const IntRange *range);

#endif
