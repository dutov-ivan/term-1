#include "test_square.h"
#include "../../src/square/square.h"
#include "../../unity/src/unity.h"

void test_find_perfect_square() {
  TEST_ASSERT_EQUAL_INT(1, is_perfect_square(4));
  TEST_ASSERT_EQUAL_INT(1, is_perfect_square(9));
  TEST_ASSERT_EQUAL_INT(1, is_perfect_square(100));
  TEST_ASSERT_EQUAL_INT(0, is_perfect_square(65));
  TEST_ASSERT_EQUAL_INT(0, is_perfect_square(123));
  TEST_ASSERT_EQUAL_INT(0, is_perfect_square(2384724));
}

void run_square_tests() {
  RUN_TEST(test_find_perfect_square);
  printf("All hello tests run successfully\n");
}
