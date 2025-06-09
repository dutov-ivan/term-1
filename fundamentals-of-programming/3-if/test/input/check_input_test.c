#include "input/input.h"
#include "unity/src/unity.h"

// Test for check_long_double_meets_restrictions function
void test_check_long_double_meets_restrictions_valid_inclusive(void) {
  long double value = 5.0;
  int result =
      check_long_double_meets_restrictions(&value, "number", 10.0, 0.0, 1, 1);
  TEST_ASSERT_EQUAL(0, result); // Expect success (0)
}

void test_check_long_double_meets_restrictions_valid_exclusive(void) {
  long double value = 5.0;
  int result =
      check_long_double_meets_restrictions(&value, "number", 10.0, 0.0, 0, 0);
  TEST_ASSERT_EQUAL(0, result); // Expect success (0)
}

void test_check_long_double_meets_restrictions_high_exclusive(void) {
  long double value = 10.0;
  int result =
      check_long_double_meets_restrictions(&value, "number", 10.0, 0.0, 0, 1);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1)
}

void test_check_long_double_meets_restrictions_low_exclusive(void) {
  long double value = 0.0;
  int result =
      check_long_double_meets_restrictions(&value, "number", 10.0, 0.0, 1, 0);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1)
}

void test_check_long_double_meets_restrictions_low_inclusive(void) {
  long double value = -1.0;
  int result =
      check_long_double_meets_restrictions(&value, "number", 10.0, 0.0, 1, 1);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1)
}

// Test for check_int_meets_restrictions function
void test_check_int_meets_restrictions_valid_inclusive(void) {
  int value = 5;
  int result = check_int_meets_restrictions(&value, "number", 10, 0, 1, 1);
  TEST_ASSERT_EQUAL(0, result); // Expect success (0)
}

void test_check_int_meets_restrictions_valid_exclusive(void) {
  int value = 5;
  int result = check_int_meets_restrictions(&value, "number", 10, 0, 0, 0);
  TEST_ASSERT_EQUAL(0, result); // Expect success (0)
}

void test_check_int_meets_restrictions_high_exclusive(void) {
  int value = 10;
  int result = check_int_meets_restrictions(&value, "number", 10, 0, 0, 1);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1)
}

void test_check_int_meets_restrictions_low_exclusive(void) {
  int value = 0;
  int result = check_int_meets_restrictions(&value, "number", 10, 0, 1, 0);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1)
}

void test_check_int_meets_restrictions_low_inclusive(void) {
  int value = -1;
  int result = check_int_meets_restrictions(&value, "number", 10, 0, 1, 1);
  TEST_ASSERT_EQUAL(1, result); // Expect failure (1)
}

void run_check_input_tests() {

  RUN_TEST(test_check_long_double_meets_restrictions_valid_inclusive);
  RUN_TEST(test_check_long_double_meets_restrictions_valid_exclusive);
  RUN_TEST(test_check_long_double_meets_restrictions_high_exclusive);
  RUN_TEST(test_check_long_double_meets_restrictions_low_exclusive);
  RUN_TEST(test_check_long_double_meets_restrictions_low_inclusive);

  RUN_TEST(test_check_int_meets_restrictions_valid_inclusive);
  RUN_TEST(test_check_int_meets_restrictions_valid_exclusive);
  RUN_TEST(test_check_int_meets_restrictions_high_exclusive);
  RUN_TEST(test_check_int_meets_restrictions_low_exclusive);
  RUN_TEST(test_check_int_meets_restrictions_low_inclusive);
}
