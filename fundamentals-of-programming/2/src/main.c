#include "../include/triangle.h"

void process_arguments(int argc, char *argv[], int *repeat_mode,
                       int *quiet_mode, int *help_mode);
void print_demo();
void read_triangle_sides(long double sides[SIDE_COUNT],
                         const char *side_labels[SIDE_COUNT],
                         const char *repeat_side_input);
void read_triangle_properties(long double properties[PROPERTY_COUNT],
                              long double a, long double b, long double c);
void print_properties(long double properties[PROPERTY_COUNT],
                      const char *property_labels[PROPERTY_COUNT],
                      long double sides[SIDE_COUNT], unsigned decimal_places);
void print_help();

int main(int argc, char *argv[]) {
  int repeat_mode = 0;
  int quiet_mode = 0;
  int help_mode = 0;

  process_arguments(argc, argv, &repeat_mode, &quiet_mode, &help_mode);

  if (help_mode) {
    print_help();
    return 0;
  }

  long double sides[SIDE_COUNT] = {0};
  long double properties[PROPERTY_COUNT] = {0};

  const char *property_labels[PROPERTY_COUNT] = {
      "Периметр трикутника:",     "Площа трикутника:",
      "Висота до сторони а:",     "Висота до сторони b:",
      "Висота до сторони c:",     "Медіана до сторони a:",
      "Медіана до сторони b:",    "Медіана до сторони c:",
      "Бісектриса до сторони a:", "Бісектриса до сторони b:",
      "Бісектриса до сторони c:"};
  const char *side_labels[SIDE_COUNT] = {"a", "b", "c"};

  const char *repeat_side_input =
      "Просимо Вас вводити лише програмно коректні сторони трикутника.\n\n";
  const char *repeat_decimal_places_input =
      "Просимо Вас вводити лише програмно коректну точність.\n\n";
  unsigned decimal_places = 0;

  do {
    if (!quiet_mode) {
      print_demo();
      printf("\n");
    }

    // Get sides from user
    do {
      read_triangle_sides(sides, side_labels, repeat_side_input);
    } while (validate_triangle(sides[0], sides[1], sides[2]) != 0);
    printf("\n");

    // Get decimal places from user
    while (read_decimal_places(&decimal_places) != 0) {
      printf("%s", repeat_decimal_places_input);
    }
    printf("\n");

    // Calculating results
    read_triangle_properties(properties, sides[0], sides[1], sides[2]);
    print_properties(properties, property_labels, sides, decimal_places);
    printf("\n");

    if (!quiet_mode) {
      if (!repeat_mode) {
        printf("\nPS: ");
        print_help();
        printf("Дякуємо за використання нашої програми!\n");
        printf("Бажаємо Вам гарного дня!\n");
        printf("\nНатисніть Enter для виходу. ");
        getchar();
      } else {
        printf("\nНатисніть будь-яку клавішу для продовження або Ctrl+C для "
               "виходу.\n\n");
        getchar();
      }
    }
  } while (repeat_mode);

  return 0;
}

void process_arguments(int argc, char *argv[], int *repeat_mode,
                       int *quiet_mode, int *help_mode) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quiet") == 0) {
      *quiet_mode = 1;
    } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--repeat") == 0) {
      *repeat_mode = 1;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      *help_mode = 1;
    }
  }
}

void print_demo() {
  printf("Вас вітає Простокутник -- інструмент, що допоможе вам знайти: \n");
  printf("\t- периметр трикутника\n");
  printf("\t- площу трикутника\n");
  printf("\t- висоти трикутника\n");
  printf("\t- медіани трикутника\n");
  printf("\t- бісектриси трикутника\n");

  printf("\nНАШІ ОБМЕЖЕННЯ:\n");
  printf("\t- максимальна довжина сторони трикутника: %Lg\n", MAX_SIDE_LENGTH);
  printf("\t- максимальна довжина сторони в символах: %u\n",
         NUMBER_LENGTH_LIMIT);
  printf("\t- мінімальна довжина сторони трикутника: %Lg\n", MIN_SIDE_LENGTH);
  printf("\t- максимальна гарантована точність (КІЛЬКІСТЬ знаків після коми) : "
         "%u\n",
         MAX_GUARANTEED_PRECISION);
  printf("\t- мінімальна гарантована точність: %u\n", MIN_GUARANTEED_PRECISION);
  printf("\t- застосовується автоматична точність: вона збільшується до тих "
         "пір, \n\t\t доки довжини не можна буде розрізнити як рядки\n");
  printf("\t- округлення НЕ застосовується \n");
  printf("\nДля того, щоб вийти з програми передчасно, натисніть Ctrl+C.\n");
}

void read_triangle_sides(long double sides[SIDE_COUNT],
                         const char *side_labels[SIDE_COUNT],
                         const char *repeat_side_input) {
  for (int i = 0; i < SIDE_COUNT; i++) {
    while (read_triangle_side(&sides[i], side_labels[i]) != 0) {
      printf("%s", repeat_side_input);
    }
  }
}

void read_triangle_properties(long double properties[PROPERTY_COUNT],
                              long double a, long double b, long double c) {
  properties[0] = get_triangle_perimeter(a, b, c);
  properties[1] = get_triangle_area(a, b, c);
  properties[2] = get_triangle_height(a, b, c);
  properties[3] = get_triangle_height(b, a, c);
  properties[4] = get_triangle_height(c, a, b);
  properties[5] = get_triangle_median(a, b, c);
  properties[6] = get_triangle_median(b, a, c);
  properties[7] = get_triangle_median(c, a, b);
  properties[8] = get_triangle_bisector(a, b, c);
  properties[9] = get_triangle_bisector(b, a, c);
  properties[10] = get_triangle_bisector(c, a, b);
}

void print_properties(long double properties[PROPERTY_COUNT],
                      const char *property_labels[PROPERTY_COUNT],
                      long double sides[SIDE_COUNT], unsigned decimal_places) {
  printf("========================================\n");
  printf("РЕЗУЛЬТАТИ ОБЧИСЛЕНЬ:\n");
  printf("========================================\n");
  for (int i = 0; i < PROPERTY_COUNT; i++) {
    print_with_additional_precision(property_labels[i], decimal_places,
                                    properties[i], sides);
  }
};

void print_help() {
  printf("Програму можна запускати з деякими параметрами!\n"
         "-q/--quiet:  вимкнути додатковий вивід у програмі\n"
         "-r/--repeat: програма повторюватиметься після виконання\n"
         "-h/--help: вивести цей текст і припинити виконання програми\n");
}
