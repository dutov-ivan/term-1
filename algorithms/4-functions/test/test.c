#include "../unity/src/unity.h"
#include "square/test_square.h"

/* Set up and tear down functions for Unity test framework */
void setUp() {}
void tearDown() {}

int main() {
  UNITY_BEGIN();
  run_square_tests();
  UNITY_END();
  printf("Create tests, buddy!\n");
  return 0;
}
