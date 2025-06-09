#include "test/input/input_test.h"
#include "test/root/root_test.h"
#include "unity/src/unity.h"

/* Set up and tear down functions for Unity test framework */
void setUp() {}
void tearDown() {}

int main() {
  UNITY_BEGIN();
  run_custom_input_tests();
  run_check_input_tests();
  run_root_tests();
  UNITY_END();
  printf("All tests run successfully\n");
  return 0;
}
