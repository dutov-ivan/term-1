#include "input_test.h"
#include "src/input/input.h" // Include the header file with read_x function
#include "unity/src/unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void mock_input(const char *input) {
  FILE *temp = fopen("test_input.txt", "w");
  if (!temp) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  fputs(input, temp);
  fclose(temp);

  temp = freopen("test_input.txt", "r", stdin);
  if (!temp) {
    perror("freopen");
    exit(EXIT_FAILURE);
  }
}

void test_replace_commas_with_dots_basic(void) {
  char input[] = "1,234.56";
  replace_commas_with_dots(input);
  TEST_ASSERT_EQUAL_STRING("1.234.56", input); // Expect "1.234.56"
}

// Test: No commas in the input string
void test_replace_commas_with_dots_no_commas(void) {
  char input[] = "123.456";
  replace_commas_with_dots(input);
  TEST_ASSERT_EQUAL_STRING("123.456", input); // Expect "123.456"
}

// Test: Multiple commas in the input string
void test_replace_commas_with_dots_multiple_commas(void) {
  char input[] = "1,23,45,67.89";
  replace_commas_with_dots(input);
  TEST_ASSERT_EQUAL_STRING("1.23.45.67.89", input); // Expect "1.23.45.67.89"
}

// Test: Valid input (unrestricted)
void test_read_long_double_valid_input_unrestricted(void) {
  long double value;
  mock_input("123.456\n"); // Simulate valid input

  int result = read_long_double(&value, "number", "num", 50, 0, 0, 0, 0, 0, 0);
  TEST_ASSERT_EQUAL(0, result);             // Expect success (0)
  TEST_ASSERT_EQUAL_FLOAT(123.456L, value); // Check the correct value
}

// Test: Valid input (restricted within range, inclusive min and max)
void test_read_long_double_valid_input_restricted(void) {
  long double value;
  mock_input("0.75\n"); // Simulate valid input within range

  int result =
      read_long_double(&value, "precision", "prec", 50, 1, 0, 1.0L, 0.5L, 1, 1);
  TEST_ASSERT_EQUAL(0, result);          // Expect success (0)
  TEST_ASSERT_EQUAL_FLOAT(0.75L, value); // Check the correct value
}

// Test: Invalid input (non-numeric characters)
void test_read_long_double_invalid_input_non_numeric(void) {
  long double value;
  mock_input("abc\n"); // Simulate invalid input

  int result = read_long_double(&value, "number", "num", 50, 0, 0, 0, 0, 0, 0);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1) due to non-numeric input
}

// Test: Input too long (exceeds max_char_count)
void test_read_long_double_input_too_long(void) {
  long double value;
  char long_input[60]; // Create a string longer than max_char_count (50)
  memset(long_input, '1', 59);
  long_input[59] = '\n';

  mock_input(long_input); // Simulate long input

  int result = read_long_double(&value, "number", "num", 50, 0, 0, 0, 0, 0, 0);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1) due to long input
}

// Test: Invalid range (value exceeds max_value, exclusive)
void test_read_long_double_exceeds_max_value_exclusive(void) {
  long double value;
  mock_input("1.5\n"); // Simulate input greater than max_value (1.0)

  int result =
      read_long_double(&value, "precision", "prec", 50, 1, 0, 1.0L, 0.5L, 0, 1);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1) due to exceeding max_value
}

// Test: Invalid range (value below min_value, exclusive)
void test_read_long_double_below_min_value_exclusive(void) {
  long double value;
  mock_input("0.4\n"); // Simulate input less than min_value (0.5)

  int result =
      read_long_double(&value, "precision", "prec", 50, 1, 0, 1.0L, 0.5L, 1, 0);
  TEST_ASSERT_EQUAL(1,
                    result); // Expect failure (1) due to being below min_value
}

// Test: Edge case for max value (exactly max_value, exclusive)
void test_read_long_double_max_value_exclusive(void) {
  long double value;
  mock_input("1.0\n"); // Simulate input exactly at max_value

  int result =
      read_long_double(&value, "precision", "prec", 50, 1, 0, 1.0L, 0.5L, 0, 1);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1), as max_value is exclusive
}

// Test: Edge case for min value (exactly min_value, exclusive)
void test_read_long_double_min_value_exclusive(void) {
  long double value;
  mock_input("0.5\n"); // Simulate input exactly at min_value

  int result =
      read_long_double(&value, "precision", "prec", 50, 1, 0, 1.0L, 0.5L, 1, 0);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1), as min_value is exclusive
}

// Test: Edge case for max value (exactly max_value, inclusive)
void test_read_long_double_max_value_inclusive(void) {
  long double value;
  mock_input("1.0\n"); // Simulate input exactly at max_value

  int result =
      read_long_double(&value, "precision", "prec", 50, 1, 0, 1.0L, 0.5L, 1, 1);
  TEST_ASSERT_EQUAL(0, result); // Expect success (0), as max_value is inclusive
}

// Test: Edge case for min value (exactly min_value, inclusive)
void test_read_long_double_min_value_inclusive(void) {
  long double value;
  mock_input("0.5\n"); // Simulate input exactly at min_value

  int result =
      read_long_double(&value, "precision", "prec", 50, 1, 0, 1.0L, 0.5L, 1, 1);
  TEST_ASSERT_EQUAL(0, result); // Expect success (0), as min_value is inclusive
}

// Test: Valid input (within range, inclusive boundaries)
void test_read_int_valid_input_inclusive(void) {
  int value;
  mock_input("5\n"); // Simulate valid integer input within range

  int result =
      read_int(&value, "number", "num", MAX_CHARS_K, 1, 0, 10, 0, 1, 1);
  TEST_ASSERT_EQUAL(0, result); // Expect success (0)
  TEST_ASSERT_EQUAL(5, value);  // Expect value to be set to 5
}

// Test: Out-of-range input (exceeding max_value, exclusive)
void test_read_int_out_of_range_high_exclusive(void) {
  int value;
  mock_input("10\n"); // Simulate input exactly at max_value (exclusive)

  int result = read_int(&value, "number", "num", MAX_CHARS_K, 1, 0, 9, 0, 0, 1);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1) due to exceeding max_value
}

// Test: Out-of-range input (below min_value, exclusive)
void test_read_int_out_of_range_low_exclusive(void) {
  int value;
  mock_input("0\n"); // Simulate input exactly at min_value (exclusive)

  int result =
      read_int(&value, "number", "num", MAX_CHARS_K, 1, 0, 10, 1, 1, 0);
  TEST_ASSERT_EQUAL(1,
                    result); // Expect failure (1) due to being below min_value
}

// Test: Invalid input (non-numeric characters)
void test_read_int_invalid_format(void) {
  int value;
  mock_input("abc\n"); // Non-integer input

  int result =
      read_int(&value, "number", "num", MAX_CHARS_K, 1, 0, 10, 0, 1, 1);
  TEST_ASSERT_EQUAL(1,
                    result); // Expect failure (1) due to invalid input format
}

// Test: Input without newline (too long)
void test_read_int_no_newline(void) {
  int value;
  mock_input("123"); // Input missing newline at the end

  int result =
      read_int(&value, "number", "num", MAX_CHARS_K, 1, 0, 10, 0, 1, 1);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1) due to missing newline
}

// Test: Input with decimal value (should fail since it's not an integer)
void test_read_int_with_decimal(void) {
  int value;
  mock_input("5.5\n"); // Simulate input with a decimal value

  int result =
      read_int(&value, "number", "num", MAX_CHARS_K, 1, 0, 10, 0, 1, 1);
  TEST_ASSERT_EQUAL(
      1, result); // Expect failure (1) because input is not an integer
}

// Test: Valid input (unrestricted mode)
void test_read_int_valid_unrestricted(void) {
  int value;
  mock_input("-100\n"); // Simulate valid integer input without restrictions

  int result = read_int(&value, "number", "num", MAX_CHARS_K, 0, 0, 0, 0, 0, 0);
  TEST_ASSERT_EQUAL(0, result);   // Expect success (0)
  TEST_ASSERT_EQUAL(-100, value); // Expect value to be set correctly
}

// Test: Valid input in quiet mode
void test_read_int_valid_quiet_mode(void) {
  int value;
  mock_input("8\n"); // Simulate valid input in quiet mode

  int result =
      read_int(&value, "number", "num", MAX_CHARS_K, 1, 1, 10, 0, 1, 1);
  TEST_ASSERT_EQUAL(0, result); // Expect success (0)
  TEST_ASSERT_EQUAL(8, value);  // Expect value to be set to 8
}

// Test: Valid input (within range, min inclusive, max exclusive)
void test_read_int_valid_range_inclusive_min_exclusive_max(void) {
  int value;
  mock_input("10\n"); // Input exactly at max_value (exclusive)

  int result =
      read_int(&value, "number", "num", MAX_CHARS_K, 1, 0, 10, 0, 0, 1);
  TEST_ASSERT_EQUAL(
      1, result); // Expect failure (1) due to max_value being exclusive
}

// Test: Valid input (exactly at min_value inclusive)
void test_read_int_valid_min_value_inclusive(void) {
  int value;
  mock_input("0\n"); // Input exactly at min_value

  int result =
      read_int(&value, "number", "num", MAX_CHARS_K, 1, 0, 10, 0, 1, 1);
  TEST_ASSERT_EQUAL(0, result); // Expect success (0), min_value is inclusive
}

// Test: Valid precision input (within range, unrestricted)
void test_read_precision_or_decimal_places_valid_precision_unrestricted(void) {
  long double precision;
  int decimal_places;

  // Simulate valid precision input
  mock_input("0.01\n");

  int result = read_precision_or_decimal_places(&precision, &decimal_places, 0,
                                                0, 1.0, 0.0001);

  TEST_ASSERT_EQUAL(0, result); // Expect success (0)
  TEST_ASSERT_EQUAL_FLOAT(0.01,
                          precision);   // Expect precision to be set correctly
  TEST_ASSERT_EQUAL(2, decimal_places); // Expect decimal places to match
}

// Test: Valid decimal places input when precision is 0
void test_read_precision_or_decimal_places_valid_decimal_places(void) {
  long double precision = 0.0;
  int decimal_places;

  // Simulate valid precision input (first input precision = 0)
  mock_input("0\n2\n"); // Precision is 0, so now decimal places will be read

  int result = read_precision_or_decimal_places(&precision, &decimal_places, 1,
                                                0, 1.0, 0.0001);

  TEST_ASSERT_EQUAL(0, result); // Expect success (0)
  TEST_ASSERT_EQUAL_FLOAT(pow(10.0L, -2),
                          precision);   // Expect precision to be 0.01 (1e-2)
  TEST_ASSERT_EQUAL(2, decimal_places); // Expect decimal places to be set to 2
}

// Test: Precision input out of range (exceeds max_value_precision)
void test_read_precision_or_decimal_places_out_of_range_precision(void) {
  long double precision;
  int decimal_places;

  // Simulate out-of-range precision input (greater than max_value_precision)
  mock_input("1.1\n");

  int result = read_precision_or_decimal_places(&precision, &decimal_places, 1,
                                                0, 1.0, 0.0001);

  TEST_ASSERT_EQUAL(
      1, result); // Expect failure (1) due to exceeding max_value_precision
}

// Test: Valid precision with quiet mode enabled
void test_read_precision_or_decimal_places_valid_quiet_mode(void) {
  long double precision;
  int decimal_places;

  // Simulate valid precision input in quiet mode
  mock_input("0.001\n");

  int result = read_precision_or_decimal_places(&precision, &decimal_places, 0,
                                                1, 1.0, 0.0001);

  TEST_ASSERT_EQUAL(0, result); // Expect success (0)
  TEST_ASSERT_EQUAL_FLOAT(0.001,
                          precision);   // Expect precision to be set correctly
  TEST_ASSERT_EQUAL(3, decimal_places); // Expect decimal places to match
}

// Test: Precision is zero, read decimal places
void test_read_precision_or_decimal_places_precision_zero(void) {
  long double precision = 0.0;
  int decimal_places;

  // Simulate valid decimal places input when precision is zero
  mock_input("0\n3\n");

  int result = read_precision_or_decimal_places(&precision, &decimal_places, 1,
                                                0, 1.0, 0.0001);

  TEST_ASSERT_EQUAL(0, result);             // Expect success (0)
  TEST_ASSERT_EQUAL(-3, log10l(precision)); // Check if precision is correctly
                                            // derived from decimal places
  TEST_ASSERT_EQUAL(3, decimal_places); // Expect decimal places to be set to 3
}

// Test: Precision out of range (below min_value_precision)
void test_read_precision_or_decimal_places_out_of_range_min(void) {
  long double precision;
  int decimal_places;

  // Simulate out-of-range precision input (below min_value_precision)
  mock_input("0.00001\n");

  int result = read_precision_or_decimal_places(&precision, &decimal_places, 1,
                                                0, 1.0, 0.0001);

  TEST_ASSERT_EQUAL(1, result); // Expect failure (1) due to precision being
                                // below min_value_precision
}

// Test: Valid input (within limit)
void test_validate_input_precision_valid_input(void) {
  int result = validate_input_precision("12345.678", 15);
  TEST_ASSERT_EQUAL(0, result); // Expect success (0)
}

// Test: Exceeding significant digits
void test_validate_input_precision_exceeding_digits(void) {
  int result = validate_input_precision("1234567890123456", 15);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1)
}

// Test: Input with zero significant digits
void test_validate_input_precision_zero_digits(void) {
  int result = validate_input_precision("0.0000000", 0);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1)
}

// Test: Input with no digits
void test_validate_input_precision_no_digits(void) {
  int result = validate_input_precision("....", 15);
  TEST_ASSERT_EQUAL(0, result); // Expect success (0)
}

// Main function to run all tests
void run_custom_input_tests() {
  RUN_TEST(test_replace_commas_with_dots_basic);
  RUN_TEST(test_replace_commas_with_dots_no_commas);
  RUN_TEST(test_replace_commas_with_dots_multiple_commas);

  RUN_TEST(test_read_long_double_valid_input_unrestricted);
  RUN_TEST(test_read_long_double_valid_input_restricted);
  RUN_TEST(test_read_long_double_invalid_input_non_numeric);
  RUN_TEST(test_read_long_double_input_too_long);
  RUN_TEST(test_read_long_double_exceeds_max_value_exclusive);
  RUN_TEST(test_read_long_double_below_min_value_exclusive);
  RUN_TEST(test_read_long_double_max_value_exclusive);
  RUN_TEST(test_read_long_double_min_value_exclusive);
  RUN_TEST(test_read_long_double_max_value_inclusive);
  RUN_TEST(test_read_long_double_min_value_inclusive);

  RUN_TEST(test_read_int_valid_input_inclusive);
  RUN_TEST(test_read_int_out_of_range_high_exclusive);
  RUN_TEST(test_read_int_out_of_range_low_exclusive);
  RUN_TEST(test_read_int_invalid_format);
  RUN_TEST(test_read_int_no_newline);
  RUN_TEST(test_read_int_with_decimal);
  RUN_TEST(test_read_int_valid_unrestricted);
  RUN_TEST(test_read_int_valid_quiet_mode);
  RUN_TEST(test_read_int_valid_range_inclusive_min_exclusive_max);
  RUN_TEST(test_read_int_valid_min_value_inclusive);

  RUN_TEST(test_read_precision_or_decimal_places_valid_precision_unrestricted);
  RUN_TEST(test_read_precision_or_decimal_places_valid_decimal_places);
  RUN_TEST(test_read_precision_or_decimal_places_out_of_range_precision);
  RUN_TEST(test_read_precision_or_decimal_places_valid_quiet_mode);
  RUN_TEST(test_read_precision_or_decimal_places_precision_zero);
  RUN_TEST(test_read_precision_or_decimal_places_out_of_range_min);

  RUN_TEST(test_validate_input_precision_valid_input);
  RUN_TEST(test_validate_input_precision_exceeding_digits);
  RUN_TEST(test_validate_input_precision_zero_digits);
  RUN_TEST(test_validate_input_precision_no_digits);
  printf("All input tests completed successfully!\n");
}
