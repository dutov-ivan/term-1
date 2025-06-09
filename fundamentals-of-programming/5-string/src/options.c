#include <stdio.h>

#include "input/input.h"
#include "main.h"
#include "stdlib.h"

#define OPTION_NUMBER_SPACING 5

void print_options() {
  printf("\n");
  for (int i = 0; i < DEFAULT_RANGE_COUNT; i++) {
    printf(GREEN "ОПЦІЯ %-*d" RESET "%s\n", OPTION_NUMBER_SPACING, i + 1,
           DEFAULT_CHAR_RANGE_NAMES[i]);
  }
}

void print_selected_options(const int *selected_options, int option_count) {
  printf("[");
  for (int i = 0; i < option_count; i++) {
    printf("%d", selected_options[i] + 1);
    // If the last element, do not print the ,
    if (i != option_count - 1) {
      printf(", ");
    }
  }
  printf("]: ");
}

int add_option(int **arr, int *count, int option) {
  (*count)++;
  int *temp = realloc(*arr, (*count) * sizeof(int));
  if (!temp) {
    show_error_memory_allocation("перевиділення пам'яті для списку опцій");
    return FAILURE;
  }
  temp[*count - 1] = option;
  *arr = temp;
  return SUCCESS;
}

int remove_option(int **arr, int *count, int index) {
  if (index < 0 || index >= *count) {
    show_error("Неприйнятний індекс для видалення опції.");
    return FAILURE;
  }
  for (int i = index; i < *count - 1; i++) {
    (*arr)[i] = (*arr)[i + 1];
  }
  (*count)--;

  if (*count == 0) {
    free(*arr);
    *arr = NULL;
  } else {
    int *temp = realloc(*arr, (*count) * sizeof(int));
    if (!temp) {
      show_error_memory_allocation(
          "перевиділення пам'яті після видалення опції");
      return FAILURE;
    }
    *arr = temp;
  }

  return SUCCESS;
}

int process_option_input(char *c, int option_upper_bound) {
  *c = getchar();
  if (*c >= '1' && *c <= option_upper_bound) {
    return *c - '0' - 1;  // Return the option as index
  }
  return -1;
}

int prompt_for_ranges(CharRange **ranges, int *range_count) {
  print_options();

  show_warning(
      "Оберіть одну-декілька опцій за цифрами. Повторний ввід цифри призведе "
      "до скасування "
      "її вибору.\n Введіть 0, коли будете вдоволені своїм набором. За "
      "замовчуванням обирається набір англійської мови.");

  const char option_upper_bound = (char)((int)'0' + DEFAULT_RANGE_COUNT);
  int option_count = 0;
  int *selected_options = NULL;
  char c;
  bool is_ended = false;

  while (!is_ended) {
    int option = process_option_input(&c, option_upper_bound);

    if (option >= 0) {  // If valid input (1 to option_upper_bound)
      bool is_repeat_found = false;
      for (int i = 0; i < option_count; i++) {
        if (selected_options[i] == option) {
          is_repeat_found = true;
          if (remove_option(&selected_options, &option_count, i) == FAILURE) {
            return FAILURE;
          }
          i--;
          break;
        }
      }
      if (!is_repeat_found) {
        if (add_option(&selected_options, &option_count, option) == FAILURE) {
          return FAILURE;
        }
      }
    } else if (c == '0') {
      is_ended = true;
      clear_stdin();
    } else if (c == ' ' || c == '\t') {
      continue;
    } else if (c == '\n') {
      print_selected_options(selected_options, option_count);
    } else {
      show_warning(
          "Усі цифри і символи, що не входять в опції, пропускаються.");
    }
  }

  if (option_count == 0) {
    selected_options = (int *)DEFAULT_OPTIONS;
    option_count = DEFAULT_OPTION_COUNT;
  }

  if (get_char_ranges_from_options(ranges, selected_options, option_count) ==
      FAILURE) {
    return FAILURE;
  }

  *range_count = option_count;
  return SUCCESS;
}
