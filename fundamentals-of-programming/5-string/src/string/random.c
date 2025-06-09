#include <stdlib.h>
#include <time.h>

#include "../input/input.h"
#include "main.h"

#ifndef MAX_BYTE_COUNT_PER_CHAR
#define MAX_BYTE_COUNT_PER_CHAR 4
#endif

const CharRange DEFAULT_CHAR_RANGES[DEFAULT_RANGE_COUNT] = {
    [DECIMALS] = {48, 57, 1},
    [SPECIAL_SYMBOLS] = {33, 47, 1},
    [ENGLISH_UPPER] = {65, 90, 1},
    [ENGLISH_LOWER] = {97, 122, 1},
    [EMOTICONS] = {128512, 128591, 4}};

const char *DEFAULT_CHAR_RANGE_NAMES[DEFAULT_RANGE_COUNT] = {
    [ENGLISH_UPPER] = "Англійські великі літери (A-Z)",
    [ENGLISH_LOWER] = "Англійські малі літери (a-z)",
    [DECIMALS] = "Десяткові цифри (0-9)",
    [SPECIAL_SYMBOLS] = "Спеціальні символи (!\"#$...)",
    [EMOTICONS] = "Смайлики"};

const int DEFAULT_OPTIONS[DEFAULT_OPTION_COUNT] = {ENGLISH_UPPER,
                                                   ENGLISH_LOWER};

void initialize_generator() { srand(time(NULL)); }

int get_random_integer_in_range(int start, int end) {
  return (rand() % (end - start + 1)) + start;
}

int get_char_ranges_from_options(CharRange **ranges, int *choices, int length) {
  *ranges = (CharRange *)malloc(length * sizeof(CharRange));
  if (*ranges == NULL) {
    show_error_memory_allocation("перетворення числових варіантів на проміжки");
    return FAILURE;
  }

  for (int i = 0; i < length; i++) {
    (*ranges)[i] = DEFAULT_CHAR_RANGES[choices[i]];
  }

  return SUCCESS;
}

int get_random_character_in_range(CharRange range, char *buffer) {
  int random_code_point = get_random_integer_in_range(range.start, range.end);
  // Convert the code point into a UTF-8 byte sequence
  if (random_code_point <= 0x7F) {
    buffer[0] = (char)random_code_point;  // 1 byte for ASCII characters
    return 1;
  } else if (random_code_point <= 0x7FF) {
    // 2 bytes for characters in the range U+0080 to U+07FF
    buffer[0] = 0xC0 | (random_code_point >> 6);
    buffer[1] = 0x80 | (random_code_point & 0x3F);
    return 2;
  } else if (random_code_point <= 0xFFFF) {
    // 3 bytes for characters in the range U+0800 to U+FFFF
    buffer[0] = 0xE0 | (random_code_point >> 12);
    buffer[1] = 0x80 | ((random_code_point >> 6) & 0x3F);
    buffer[2] = 0x80 | (random_code_point & 0x3F);
    return 3;
  } else {
    // 4 bytes for characters in the range U+10000 to U+10FFFF
    buffer[0] = 0xF0 | (random_code_point >> 18);
    buffer[1] = 0x80 | ((random_code_point >> 12) & 0x3F);
    buffer[2] = 0x80 | ((random_code_point >> 6) & 0x3F);
    buffer[3] = 0x80 | (random_code_point & 0x3F);
    return 4;
  }
}

// Ensures proper probability for different ranges
int calculate_cumulative_range_ends(int **cumulative_range_ends,
                                    CharRange *ranges, int range_count) {
  *cumulative_range_ends = (int *)malloc(range_count * sizeof(int));
  if (*cumulative_range_ends == NULL) {
    show_error_memory_allocation("масиву кінцевих точок проміжків");
    return FAILURE;
  }

  int sum = 0;
  for (int i = 0; i < range_count; i++) {
    int range_size = ranges[i].end - ranges[i].start + 1;
    sum += range_size;
    (*cumulative_range_ends)[i] = sum;
  }

  return SUCCESS;
}

int get_random_fixed_length_string_from_ranges(char **string, int length,
                                               CharRange *ranges,
                                               int range_count,
                                               int *cumulative_range_ends) {
  int max_byte_count = 1;
  for (int i = 0; i < range_count; i++) {
    if (ranges[i].byte_count > max_byte_count) {
      max_byte_count = ranges[i].byte_count;
    }
  }

  *string = (char *)malloc((length * max_byte_count + 1) *
                           sizeof(char));  // 4 bytes per UTF-8 chars
                                           // +1 for null terminator
  if (*string == NULL) {
    show_error_memory_allocation("створення випадкового рядка");
    return FAILURE;
  }

  size_t generated_char_count = 0;
  size_t generated_byte_count = 0;

  char utf8_buffer[max_byte_count];

  while (generated_char_count < length) {
    int random_char_index =
        get_random_integer_in_range(1, cumulative_range_ends[range_count - 1]);

    int chosen_range_index = 0;
    while (random_char_index > cumulative_range_ends[chosen_range_index]) {
      chosen_range_index++;
    }

    int bytes_written =
        get_random_character_in_range(ranges[chosen_range_index], utf8_buffer);

    // Copy the UTF-8 character into the string
    for (int i = 0; i < bytes_written; i++) {
      (*string)[generated_byte_count++] = utf8_buffer[i];
    }
    generated_char_count++;
  }

  (*string)[generated_byte_count] = '\0';
  return SUCCESS;
}
