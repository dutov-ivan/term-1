#include "../include/utils.h"
void replace_commas_with_dots(char *str) {
  while (*str) {
    if (*str == ',') {
      *str = '.';
    }
    str++;
  }
}

void flush_input_buffer() {
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF) {
    // Keep consuming the remaining characters until newline or EOF
  }
}

void truncate_to_precision(char *formatted_value, unsigned final_precision) {
  for (int i = 0; formatted_value[i] != '\0'; i++) {
    if (formatted_value[i] == '.') {
      // If we reach the decimal point, truncate after the required precision
      if (i + final_precision + 1 < strlen(formatted_value)) {
        formatted_value[i + final_precision + 1] = '\0';
      }
      break;
    }
  }
}
