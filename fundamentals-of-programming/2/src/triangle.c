#include "../include/triangle.h"
#include "../include/utils.h"

// Core Triangle Calculations

long double get_triangle_perimeter(long double a, long double b,
                                   long double c) {
  return (a + b + c);
}

long double get_triangle_area(long double a, long double b, long double c) {
  long double p = get_triangle_perimeter(a, b, c) / 2.0;
  return sqrtl(p * (p - a) * (p - b) * (p - c));
}

long double get_triangle_height(long double side, long double b,
                                long double c) {
  return get_triangle_area(side, b, c) * 2.0 / side;
}

long double get_triangle_median(long double side, long double b,
                                long double c) {
  return sqrtl(2 * b * b + 2 * c * c - side * side) / 2.0;
}

long double get_triangle_bisector(long double side, long double b,
                                  long double c) {
  long double p = get_triangle_perimeter(side, b, c) / 2;
  return 2 * sqrtl(b * c * p * (p - side)) / (b + c);
}

// Triangle validation
int validate_triangle(long double a, long double b, long double c) {
  if (a <= 0) {
    printf("\nПОМИЛКА! Довжина сторони a має бути додатною!\n");
    return 1;
  }
  if (b <= 0) {
    printf("\nПОМИЛКА! Довжина сторони b має бути додатною!\n");
    return 1;
  }

  if (c <= 0) {
    printf("\nПОМИЛКА! Довжина сторони c має бути додатною!\n");
    return 1;
  }

  if (a + b <= c) {
    printf("\nПОМИЛКА! Сума довжин сторін a та b має бути більшою за довжину "
           "сторони c!\n");
    return 1;
  }

  if (a + c <= b) {
    printf("\nПОМИЛКА! Сума довжин сторін a та c має бути більшою за довжину "
           "сторони b!\n");
    return 1;
  }

  if (b + c <= a) {
    printf("\nПОМИЛКА! Сума довжин сторін b та c має бути більшою за довжину "
           "сторони a!\n");
    return 1;
  }

  return 0;
}

// Input and precision handling functions

int read_triangle_side(long double *side, const char *side_name) {
  char input[NUMBER_LENGTH_LIMIT + 1];
  char *endptr;

  printf("Введіть, будь ласка, довжину сторони %s: ", side_name);

  if (!fgets(input, sizeof(input), stdin)) {
    perror("\nПОМИЛКА! Не вдалося прочитати ввід!\n");
    return 1;
  }

  if (input[strlen(input) - 1] != '\n') {
    printf("\nПОМИЛКА! Довжина сторони %s в символах має бути меншою за %u.\n",
           side_name, NUMBER_LENGTH_LIMIT);
    flush_input_buffer();
    return 1;
  }

  replace_commas_with_dots(input);
  *side = strtold(input, &endptr);

  // Check if conversion failed or extra characters exist
  if (endptr == input || *endptr != '\n') {
    printf("\nПОМИЛКА! Довжина сторони %s має бути числом і не містити "
           "додаткових символів!\n",
           side_name);
    return 1;
  }

  // Check for side length constraints
  if (*side < 0) {
    printf("\nПОМИЛКА! Довжина сторони %s має бути додатною!\n", side_name);
    return 1;
  }
  if (*side > MAX_SIDE_LENGTH) {
    printf("\nПОМИЛКА! Довжина сторони %s має бути менша-рівна %Lg.\n",
           side_name, MAX_SIDE_LENGTH);
    return 1;
  }
  if (*side < MIN_SIDE_LENGTH - TOLERANCE) {
    printf("\nПОМИЛКА! Довжина сторони %s має бути більша-рівна %Lg.\n",
           side_name, MIN_SIDE_LENGTH);
    return 1;
  }

  return 0;
}

int read_decimal_places(unsigned *decimal_places) {
  printf("Введіть точність від 1 до %u: ", MAX_GUARANTEED_PRECISION);

  char input[NUMBER_LENGTH_LIMIT + 1];
  char *endptr;
  long value;

  if (!fgets(input, sizeof(input), stdin)) {
    printf("\nПОМИЛКА! Не вдалося прочитати ввід!\n");
    return 1;
  }

  if (input[strlen(input) - 1] != '\n') {
    printf(
        "\nПОМИЛКА! Максимальна довжина значення точності в символах складає "
        "%u.\n",
        NUMBER_LENGTH_LIMIT);
    flush_input_buffer();
    return 1;
  }

  value = strtol(input, &endptr, 10);

  if (endptr == input || *endptr != '\n') {
    printf("\nПОМИЛКА! Точність має бути числом.\n");
    return 1;
  }

  if (value > MAX_GUARANTEED_PRECISION) {
    printf("\nПОМИЛКА! Точність має бути менша-рівна %u.\n",
           MAX_GUARANTEED_PRECISION);
    return 1;
  }

  if (value < MIN_GUARANTEED_PRECISION) {
    printf("\nПОМИЛКА! Точність має бути більша-рівна %u.\n",
           MIN_GUARANTEED_PRECISION);
    return 1;
  }

  *decimal_places = (unsigned)value;
  return 0;
}

// Precision Adjustment Functions

static int numbers_are_equal_by_string(const long double value1,
                                       const long double value2,
                                       unsigned precision) {
  char buffer1[NUMBER_LENGTH_LIMIT], buffer2[NUMBER_LENGTH_LIMIT];
  snprintf(buffer1, sizeof(buffer1), "%.*Lf", precision, value1);
  snprintf(buffer2, sizeof(buffer2), "%.*Lf", precision, value2);
  return strcmp(buffer1, buffer2) == 0;
}

void print_with_additional_precision(const char *text, unsigned int precision,
                                     long double value,
                                     long double sides[SIDE_COUNT]) {
  char formatted_value[NUMBER_LENGTH_LIMIT];
  unsigned needed_precision = precision;

  for (int i = 0; i < SIDE_COUNT; i++) {
    if (!numbers_are_equal_by_string(value, sides[i], needed_precision)) {
      continue;
    }

    while (numbers_are_equal_by_string(value, sides[i], needed_precision)) {
      if (needed_precision == MAX_ACTUAL_PRECISION) {
        break;
      }
      needed_precision++;
    }
  }

  unsigned final_precision =
      (needed_precision > precision) ? needed_precision : precision;

  // Extra digit for rounding prevention
  snprintf(formatted_value, sizeof(formatted_value), "%.*Lf",
           final_precision + 1, value);
  truncate_to_precision(formatted_value, final_precision);

  printf("%s %s\n", text, formatted_value);
}
