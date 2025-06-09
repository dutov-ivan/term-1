#ifndef MAIN_H
#define MAIN_H

#define DEFAULT_OPTION_COUNT 2  // ENGLISH_UPPER and ENGLISH_LOWER

#define SUCCESS 0
#define FAILURE 1

typedef struct {
  int start;
  int end;
  int byte_count;
} CharRange;

typedef enum {
  ENGLISH_UPPER,
  ENGLISH_LOWER,
  DECIMALS,
  SPECIAL_SYMBOLS,
  EMOTICONS,
  DEFAULT_RANGE_COUNT
} DefaultCharRangeNames;

extern const CharRange DEFAULT_CHAR_RANGES[DEFAULT_RANGE_COUNT];
extern const char *DEFAULT_CHAR_RANGE_NAMES[DEFAULT_RANGE_COUNT];
extern const int DEFAULT_OPTIONS[DEFAULT_OPTION_COUNT];

void initialize_generator();
void print_string_list(char **string_list, const char *prefix, int length);
void sort_inplace_string_list(char **string_list, int low, int high);
int get_random_integer_in_range(int start, int end);
int get_char_ranges_from_options(CharRange **ranges, int *choices, int length);
int get_random_fixed_length_string_from_ranges(char **string, int length,
                                               CharRange *ranges,
                                               int range_count,
                                               int *cumulative_range_ends);

int calculate_cumulative_range_ends(int **cumulative_range_ends,
                                    CharRange *ranges, int range_count);
int prompt_for_ranges(CharRange **ranges, int *range_count);

#endif
