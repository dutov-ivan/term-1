#include "../unity/src/unity.h"
#include "input/test_input.h"

/* Set up and tear down functions for Unity test framework */
void setUp() {}
void tearDown() {}

int main() {
  UNITY_BEGIN();
  printf("Running input tests\n");
  test_common_utils();
  test_float_utils();
  UNITY_END();
  printf("All tests run successfully\n");
  return 0;
}
