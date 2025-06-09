#include "main.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "input/input.h"

#define MAX_INT_INPUT_CHAR_COUNT 4
#define MIN_STRING_LENGTH 1
#define MAX_STRING_LENGTH 1000

#define MIN_STRING_COUNT 1
#define MAX_STRING_COUNT 1000

void print_demo() {
  printf(
      "\nВас вітає програма Sort It Out, що відсортовує випадково згенеровані "
      "або введені користувачем рядки.\n");

  show_warning("Наші обмеження такі:");
  printf("%d ≤ кількість рядків, що вводяться ≤ %d\n", MIN_STRING_COUNT,
         MAX_STRING_COUNT);
  printf("%d ≤ максимальна довжина окремого рядка у символах ≤ %d\n",
         MIN_STRING_LENGTH, MAX_STRING_LENGTH);
  show_warning(
      "Можливо згенерувати тільки рядки з цифрами, спецсимволами, смайликами, "
      "англійськими буквами. Символи сортуються за таблицею UTF-8\n");
}

int main() {
  bool is_repeat = false;
  print_demo();

  do {
    int string_count = 0;
    int max_user_string_length = 0;
    while (read_int_and_validate(&string_count, "Кількість рядків для обробки",
                                 "Кількість рядків", MAX_INT_INPUT_CHAR_COUNT,
                                 true, MAX_STRING_COUNT, MIN_STRING_COUNT, true,
                                 false) == FAILURE);

    char **string_list = (char **)malloc(string_count * sizeof(char *));
    if (string_list == NULL) {
      show_error_memory_allocation("списку рядків");
      return FAILURE;
    }

    while (read_int_and_validate(
               &max_user_string_length, "Максимальна довжина рядка",
               "Максимальна довжина рядка", MAX_INT_INPUT_CHAR_COUNT, true,
               MAX_STRING_LENGTH, MIN_STRING_LENGTH, true, false) == FAILURE);

    bool is_random_mode =
        ask_question("Чи хочете ви увімкнути випадковий режим?");

    if (is_random_mode) {
      initialize_generator();
      bool is_dynamic_length = ask_question(
          "Чи можуть згенеровані рядки мати менше знаків, аніж %d?",
          max_user_string_length);

      CharRange *chosen_ranges;
      int range_count;

      prompt_for_ranges(&chosen_ranges, &range_count);

      // Calculating before loop to save on performance
      int *cumulative_range_ends;
      calculate_cumulative_range_ends(&cumulative_range_ends, chosen_ranges,
                                      range_count);

      for (int i = 0; i < string_count; i++) {
        int string_length = (is_dynamic_length) ? get_random_integer_in_range(
                                                      1, max_user_string_length)
                                                : max_user_string_length;

        char *random_string;
        get_random_fixed_length_string_from_ranges(
            &random_string, string_length, chosen_ranges, range_count,
            cumulative_range_ends);
        string_list[i] = random_string;
      }

      print_string_list(string_list, "Рядок", string_count);
    } else {
      for (int i = 0; i < string_count; i++) {
        while (read_string_within_length(&string_list[i], "Рядок",
                                         max_user_string_length, true,
                                         i + 1) == FAILURE);
      }
    }

    bool is_ready = false;
    do {
      is_ready = ask_question("Чи готові ви побачити відсортовані рядки?");
      if (!is_ready) {
        printf("Гаразд, ми зачекаємо Вас!\n");
      }
    } while (!is_ready);

    // Sorting strings and outputting to user
    sort_inplace_string_list(string_list, 0, string_count - 1);
    print_string_list(string_list, "Відсортований рядок", string_count);

    // Clearing memory for next iteration
    for (int i = 0; i < string_count; i++) {
      free(string_list[i]);
    }
    free(string_list);

    is_repeat = ask_question("Бажаєте повторити програму?");
  } while (is_repeat);

  show_success("Дякуємо за використання нашої програми!");

  return SUCCESS;
}
