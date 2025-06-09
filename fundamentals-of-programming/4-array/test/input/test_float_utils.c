#include <string.h>

#include "../../src/input/input.h"
#include "../../unity/src/unity.h"
#include "test_input.h"

#ifndef TOLERANCE
#define TOLERANCE 1e-7
#endif

// Test for check_float_meets_restrictions function
void test_check_float_meets_restrictions_valid_inclusive(void) {
  float value = 5.0;
  int result = check_float_meets_restrictions(&value, "number", 10.0, 0.0, 1, 1,
                                              TOLERANCE);
  TEST_ASSERT_EQUAL(0, result);  // Expect success (0)
}

void test_check_float_meets_restrictions_valid_exclusive(void) {
  float value = 5.0;
  int result = check_float_meets_restrictions(&value, "number", 10.0, 0.0, 0, 0,
                                              TOLERANCE);
  TEST_ASSERT_EQUAL(0, result);  // Expect success (0)
}

void test_check_float_meets_restrictions_high_exclusive(void) {
  float value = 10.0;
  int result = check_float_meets_restrictions(&value, "number", 10.0, 0.0, 0, 1,
                                              TOLERANCE);
  TEST_ASSERT_EQUAL(1, result);  // Expect failure (1)
}

void test_check_float_meets_restrictions_low_exclusive(void) {
  float value = 0.0;
  int result = check_float_meets_restrictions(&value, "number", 10.0, 0.0, 1, 0,
                                              TOLERANCE);
  TEST_ASSERT_EQUAL(1, result);  // Expect failure (1)
}

void test_check_float_meets_restrictions_low_inclusive(void) {
  float value = -1.0;
  int result = check_float_meets_restrictions(&value, "number", 10.0, 0.0, 1, 1,
                                              TOLERANCE);
  TEST_ASSERT_EQUAL(1, result);  // Expect failure (1)
}

// Test: Valid input (unrestricted)
void test_read_float_valid_input_unrestricted(void) {
  float value;
  mock_input("123.456\n");  // Simulate valid input

  int result =
      read_float(&value, "number", "num", 50, 0, 0, 0, 0, 0, TOLERANCE, 0);
  TEST_ASSERT_EQUAL(0, result);              // Expect success (0)
  TEST_ASSERT_EQUAL_FLOAT(123.456f, value);  // Check the correct value
}

// Test: Valid input (restricted within range, inclusive min and max)
void test_read_float_valid_input_restricted(void) {
  float value;
  mock_input("0.75\n");  // Simulate valid input within range

  int result = read_float(&value, "precision", "prec", 50, 1, 1.0f, 0.5f, 1, 1,
                          TOLERANCE, 0);
  TEST_ASSERT_EQUAL(0, result);           // Expect success (0)
  TEST_ASSERT_EQUAL_FLOAT(0.75f, value);  // Check the correct value
}

// Test: Invalid input (non-numeric characters)
void test_read_float_invalid_input_non_numeric(void) {
  float value;
  mock_input("abc\n");  // Simulate invalid input

  int result =
      read_float(&value, "number", "num", 50, 0, 0, 0, 0, 0, TOLERANCE, 0);
  TEST_ASSERT_EQUAL(1, result);  // Expect failure (1) due to non-numeric input
}

// Test: Input too long (exceeds max_char_count)
void test_read_float_input_too_long(void) {
  float value;
  char long_input[60];  // Create a string longer than max_char_count (50)
  memset(long_input, '1', 59);
  long_input[59] = '\n';

  mock_input(long_input);  // Simulate long input

  int result =
      read_float(&value, "number", "num", 50, 0, 0, 0, 0, 0, TOLERANCE, 0);
  TEST_ASSERT_EQUAL(1, result);  // Expect failure (1) due to long input
}

// Test: Invalid range (value exceeds max_value, exclusive)
void test_read_float_exceeds_max_value_exclusive(void) {
  float value;
  mock_input("1.5\n");  // Simulate input greater than max_value (1.0)

  int result = read_float(&value, "precision", "prec", 50, 1, 1.0f, 0.5f, 0, 1,
                          TOLERANCE, 0);
  TEST_ASSERT_EQUAL(1,
                    result);  // Expect failure (1) due to exceeding max_value
}

// Test: Invalid range (value below min_value, exclusive)
void test_read_float_below_min_value_exclusive(void) {
  float value;
  mock_input("0.4\n");  // Simulate input less than min_value (0.5)

  int result = read_float(&value, "precision", "prec", 50, 1, 1.0f, 0.5f, 1, 0,
                          TOLERANCE, 0);
  TEST_ASSERT_EQUAL(1,
                    result);  // Expect failure (1) due to being below min_value
}

// Test: Edge case for max value (exactly max_value, exclusive)
void test_read_float_max_value_exclusive(void) {
  float value;
  mock_input("1.0\n");  // Simulate input exactly at max_value

  int result = read_float(&value, "precision", "prec", 50, 1, 1.0f, 0.5f, 0, 1,
                          TOLERANCE, 0);
  TEST_ASSERT_EQUAL(1,
                    result);  // Expect failure (1), as max_value is exclusive
}

// Test: Edge case for min value (exactly min_value, exclusive)
void test_read_float_min_value_exclusive(void) {
  float value;
  mock_input("0.5\n");  // Simulate input exactly at min_value

  int result = read_float(&value, "precision", "prec", 50, 1, 1.0f, 0.5f, 1, 0,
                          TOLERANCE, 0);
  TEST_ASSERT_EQUAL(1,
                    result);  // Expect failure (1), as min_value is exclusive
}

// Test: Edge case for max value (exactly max_value, inclusive)
void test_read_float_max_value_inclusive(void) {
  float value;
  mock_input("1.0\n");  // Simulate input exactly at max_value

  int result = read_float(&value, "precision", "prec", 50, 1, 1.0f, 0.5f, 1, 1,
                          TOLERANCE, 0);
  TEST_ASSERT_EQUAL(0,
                    result);  // Expect success (0), as max_value is inclusive
}

// Test: Edge case for min value (exactly min_value, inclusive)
void test_read_float_min_value_inclusive(void) {
  float value;
  mock_input("0.5\n");  // Simulate input exactly at min_value

  int result = read_float(&value, "precision", "prec", 50, 1, 1.0f, 0.5f, 1, 1,
                          TOLERANCE, 0);
  TEST_ASSERT_EQUAL(0,
                    result);  // Expect success (0), as min_value is inclusive
}

void test_float_utils() {
  RUN_TEST(test_check_float_meets_restrictions_valid_inclusive);
  RUN_TEST(test_check_float_meets_restrictions_valid_exclusive);
  RUN_TEST(test_check_float_meets_restrictions_high_exclusive);
  RUN_TEST(test_check_float_meets_restrictions_low_exclusive);
  RUN_TEST(test_check_float_meets_restrictions_low_inclusive);

  RUN_TEST(test_read_float_valid_input_unrestricted);
  RUN_TEST(test_read_float_valid_input_restricted);
  RUN_TEST(test_read_float_invalid_input_non_numeric);
  RUN_TEST(test_read_float_input_too_long);
  RUN_TEST(test_read_float_exceeds_max_value_exclusive);
  RUN_TEST(test_read_float_below_min_value_exclusive);
  RUN_TEST(test_read_float_max_value_exclusive);
  RUN_TEST(test_read_float_min_value_exclusive);
  RUN_TEST(test_read_float_max_value_inclusive);
  RUN_TEST(test_read_float_min_value_inclusive);
}
