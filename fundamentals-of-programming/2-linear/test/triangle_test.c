#include "../src/triangle.c"
#include "../src/utils.c"
#include "../unity/src/unity.h"
#include <unistd.h>

/* Set up and tear down functions for Unity test framework */
void setUp() {}
void tearDown() {}

/* Mock input function for testing individual side reading */
void mock_input_single_side(const char *input) {
  int pipe_fds[2];
  if (pipe(pipe_fds) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  write(pipe_fds[1], input, strlen(input));
  close(pipe_fds[1]); // Close write end of the pipe

  dup2(pipe_fds[0], STDIN_FILENO);
  close(pipe_fds[0]); // Close read end of the pipe
}

/* Test cases for reading triangle sides one by one */
void test_read_triangle_side_valid_whole() {
  long double a, b, c;
  mock_input_single_side("3\n"); // Mock input for side a
  TEST_ASSERT_EQUAL_INT(0, read_triangle_side(&a, "a"));

  mock_input_single_side("122\n"); // Mock input for side b
  TEST_ASSERT_EQUAL_INT(0, read_triangle_side(&b, "b"));

  mock_input_single_side("298131\n"); // Mock input for side c
  TEST_ASSERT_EQUAL_INT(0, read_triangle_side(&c, "c"));

  TEST_ASSERT_EQUAL_DOUBLE(3.0, a);
  TEST_ASSERT_EQUAL_DOUBLE(122.0, b);
  TEST_ASSERT_EQUAL_DOUBLE(298131.0, c);
}

void test_read_triangle_side_valid_float() {
  long double a, b, c;
  mock_input_single_side("1.3\n"); // Mock input for side a
  TEST_ASSERT_EQUAL_INT(0, read_triangle_side(&a, "a"));

  mock_input_single_side("2.7\n"); // Mock input for side b
  TEST_ASSERT_EQUAL_INT(0, read_triangle_side(&b, "b"));

  mock_input_single_side("3.2\n"); // Mock input for side c
  TEST_ASSERT_EQUAL_INT(0, read_triangle_side(&c, "c"));

  TEST_ASSERT_EQUAL_DOUBLE(1.3, a);
  TEST_ASSERT_EQUAL_DOUBLE(2.7, b);
  TEST_ASSERT_EQUAL_DOUBLE(3.2, c);
}

void test_read_triangle_side_negative_value() {
  long double a, b, c;
  mock_input_single_side("-3.12\n"); // Negative value for side a
  TEST_ASSERT_EQUAL_INT(1, read_triangle_side(&a, "a"));

  mock_input_single_side("-0\n"); // Valid input for side b
  TEST_ASSERT_EQUAL_INT(1, read_triangle_side(&b, "b"));

  mock_input_single_side("-5e2\n"); // Valid input for side c
  TEST_ASSERT_EQUAL_INT(1, read_triangle_side(&c, "c"));
}

void test_read_triangle_side_invalid_triangle() {
  long double a, b, c;
  mock_input_single_side("3\n");
  TEST_ASSERT_EQUAL_INT(0, read_triangle_side(&a, "a"));

  mock_input_single_side("3\n");
  TEST_ASSERT_EQUAL_INT(0, read_triangle_side(&b, "b"));

  mock_input_single_side("7\n");
  TEST_ASSERT_EQUAL_INT(0, read_triangle_side(&c, "c"));

  TEST_ASSERT_EQUAL_INT(1, validate_triangle(a, b, c));
}

void test_read_triangle_side_exceeds_limits() {
  long double a, b, c;
  mock_input_single_side("1e10\n"); // Large side a
  TEST_ASSERT_EQUAL_INT(1, read_triangle_side(&a, "a"));

  mock_input_single_side("56000000\n"); // Large side b
  TEST_ASSERT_EQUAL_INT(1, read_triangle_side(&b, "b"));

  mock_input_single_side("1e-4\n"); // Side c too small
  TEST_ASSERT_EQUAL_INT(1, read_triangle_side(&c, "c"));
}

void test_read_triangle_side_string_overflow() {
  long double a, b, c;
  mock_input_single_side(
      "1.00000000000000000000000000000000000000000000000000000000000");
  TEST_ASSERT_EQUAL_INT(1, read_triangle_side(&a, "a"));

  mock_input_single_side(
      "31.00000000000000000000000000000000000000000000000000000000000");
  TEST_ASSERT_EQUAL_INT(1, read_triangle_side(&b, "b"));

  mock_input_single_side(
      "1.6666666666666666666666666666666666666666666666666666666666666e-1");
  TEST_ASSERT_EQUAL_INT(1, read_triangle_side(&c, "c"));
}

void test_read_triangle_side_invalid_input() {
  long double a;
  mock_input_single_side("sussy baka\n"); // Invalid input for side a
  TEST_ASSERT_EQUAL_INT(1, read_triangle_side(&a, "a"));
}

/* Main function to run all tests */
int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_read_triangle_side_valid_whole);
  RUN_TEST(test_read_triangle_side_valid_float);
  RUN_TEST(test_read_triangle_side_negative_value);
  RUN_TEST(test_read_triangle_side_invalid_triangle);
  RUN_TEST(test_read_triangle_side_exceeds_limits);
  RUN_TEST(test_read_triangle_side_invalid_input);

  UNITY_END();
  printf("Tests completed successfully!\n");
  return 0;
}
