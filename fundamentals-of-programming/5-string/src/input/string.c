#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input/input.h"

void prompt_user_input(const char *name, int number, bool is_numbered) {
  if (is_numbered) {
    printf("%s %d: ", name, number);
  } else {
    printf("%s: ", name);
  }
}

void print_string_list(char **string_list, const char *prefix, int length) {
  printf("\n");
  int index_width = 1;
  int remaining_length = length;
  while (remaining_length > 1) {
    remaining_length /= 10;
    index_width++;
  }

  for (int list_index = 0; list_index < length; list_index++) {
    printf("%s %-*d: %s\n", prefix, index_width, list_index + 1,
           string_list[list_index]);
  }
}

void handle_input_error(char **value, const char *name, int max_char_count) {
  free(*value);
  show_error_overlength(name, max_char_count);
  clear_stdin();
}

bool is_utf8_lead_byte(unsigned char c) { return (c & 0xC0) != 0x80; }

int append_char_to_buffer(char **value, int c, size_t *byte_count,
                          size_t *real_char_count, size_t *buffer_size,
                          size_t max_char_count) {
  // Expand the buffer if needed
  if (*byte_count + 1 >= *buffer_size) {
    size_t new_buffer_size = *buffer_size * 2;
    char *new_buffer = realloc(*value, new_buffer_size);
    if (new_buffer == NULL) {
      return FAILURE;
    }
    *value = new_buffer;
    *buffer_size = new_buffer_size;
  }

  (*value)[(*byte_count)++] = c;

  if (is_utf8_lead_byte((unsigned char)c)) {
    (*real_char_count)++;
    if (*real_char_count > max_char_count) {
      return FAILURE;
    }
  }

  return SUCCESS;
}

int read_string_within_length(char **value, const char *name,
                              int max_char_count, bool is_numbered,
                              int number) {
  size_t buffer_size = max_char_count;
  size_t real_char_count = 0;
  size_t byte_count = 0;

  prompt_user_input(name, number, is_numbered);

  *value = (char *)malloc(buffer_size);
  if (*value == NULL) {
    show_error("Не вдалося виділити пам'ять для створення рядка");
    return FAILURE;
  }

  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    if (append_char_to_buffer(value, c, &byte_count, &real_char_count,
                              &buffer_size, max_char_count) == FAILURE) {
      handle_input_error(value, name, max_char_count);
      return FAILURE;
    }
  }

  (*value)[byte_count] = '\0';

  // Shrink the buffer to fit the string
  char *shrunk_value = realloc(*value, byte_count + 1);
  if (shrunk_value != NULL) {
    *value = shrunk_value;
  }

  return SUCCESS;
}
