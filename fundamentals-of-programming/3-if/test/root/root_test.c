#include "root_test.h"
#include "src/root/root.h"
#include "unity/src/unity.h"

void test_approximate_positive_x_positive_k() {
  const long double e_target = 1e-10L;
  const unsigned int decimal_places = 7;
  long double result = 0;

  result = approximate_kth_root(2, 2, e_target, decimal_places, 0, 0,
                                DEFAULT_ITERATION_LIMIT);
  TEST_ASSERT_FLOAT_WITHIN(e_target, 1.41421356237L, result);
  result = approximate_kth_root(2, 3, e_target, decimal_places, 0, 0,
                                DEFAULT_ITERATION_LIMIT);
  TEST_ASSERT_FLOAT_WITHIN(e_target, 1.25992104989L, result);
}

void test_approximate_positive_x_negative_k() {
  const long double e_target = 1e-10L;
  const unsigned int decimal_places = 7;
  long double result = 0;

  result = approximate_kth_root(3, -2, e_target, decimal_places, 0, 0,
                                DEFAULT_ITERATION_LIMIT);
  TEST_ASSERT_FLOAT_WITHIN(e_target, 0.57735026919L, result);
  result = approximate_kth_root(2, -3, e_target, decimal_places, 0, 0,
                                DEFAULT_ITERATION_LIMIT);
  TEST_ASSERT_FLOAT_WITHIN(e_target, 0.79370052598L, result);
}

void test_approximate_negative_x_positive_k() {
  const long double e_target = 1e-10L;
  const unsigned int decimal_places = 7;
  long double result = 0;

  result = approximate_kth_root(-2, 2, e_target, decimal_places, 0, 0,
                                DEFAULT_ITERATION_LIMIT);
  TEST_ASSERT_FLOAT_WITHIN(e_target, INFINITY, result);
  result = approximate_kth_root(-2, 3, e_target, decimal_places, 0, 0,
                                DEFAULT_ITERATION_LIMIT);
  TEST_ASSERT_FLOAT_WITHIN(e_target, -1.25992104989L, result);
}

void test_approximate_negative_x_negative_k() {
  const long double e_target = 1e-10L;
  const unsigned int decimal_places = 7;
  long double result = 0;

  result = approximate_kth_root(-2, -3, e_target, decimal_places, 0, 0,
                                DEFAULT_ITERATION_LIMIT);
  TEST_ASSERT_FLOAT_WITHIN(e_target, -0.79370052598L, result);
}

/* Main function to run all tests */
void run_root_tests() {
  RUN_TEST(test_approximate_positive_x_positive_k);
  RUN_TEST(test_approximate_positive_x_negative_k);
  RUN_TEST(test_approximate_negative_x_positive_k);
  RUN_TEST(test_approximate_negative_x_negative_k);
  printf("Root tests completed successfully!\n");
}
