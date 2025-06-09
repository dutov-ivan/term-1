#include <string.h>

#include "../main.h"

void swap_strings(char **string1, char **string2) {
  char *temp = *string1;
  *string1 = *string2;
  *string2 = temp;
}

int partition_strings(char **string_list, int low, int high) {
  const char *pivot = string_list[high];  // Choose the last item as pivot
  int i = low - 1;  // Pointer for the next element to swap with

  for (int j = low; j < high; j++) {
    if (strcmp(pivot, string_list[j]) > 0) {
      i++;
      swap_strings(&string_list[i],
                   &string_list[j]);  // Swap strings lexicographically less
    }
  }
  swap_strings(&string_list[i + 1],
               &string_list[high]);  // Place the pivot in correct position
  return i + 1;
}

// Implementation of quicksort for strings
void sort_inplace_string_list(char **string_list, int low, int high) {
  if (low < high) {
    int partition_index = partition_strings(string_list, low, high);

    sort_inplace_string_list(string_list, low, partition_index - 1);
    sort_inplace_string_list(string_list, partition_index + 1, high);
  }
}
