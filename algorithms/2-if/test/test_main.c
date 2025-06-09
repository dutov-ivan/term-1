#include "../src/geometry.c" // Include the implementation of your functions
#include "../unity/src/unity.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Set up and tear down functions for Unity test framework */
void setUp() {}
void tearDown() {}

/* Mock input function for testing */
void mock_input(const char *input) {
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

int check_area(double x, double y, double x1, double y1, double r, double k,
               double b) {
  return is_in_circle(x, y, x1, y1, r) && is_below_line(x, y, k, b) &&
         is_above_line(x, y, -k, -b);
}

/* Test cases for is_in_circle */
void test_is_in_circle() {
  TEST_ASSERT_EQUAL(1, is_in_circle(1, 1, 1, 1, 1));  // Inside circle
  TEST_ASSERT_EQUAL(1, is_in_circle(1, 1, 1, 1, 2));  // Inside bigger circle
  TEST_ASSERT_EQUAL(0, is_in_circle(1, 1, -1, 1, 1)); // Outside circle
  TEST_ASSERT_EQUAL(0, is_in_circle(4, 2, 1, 1, 2));  // Outside circle
}

/* Test cases for is_below_line */
void test_is_below_line() {
  TEST_ASSERT_EQUAL(0, is_below_line(1, 1, 1, 0)); // Point is on the line
  TEST_ASSERT_EQUAL(1, is_below_line(1, 1, 1, 2)); // Point is below line
  TEST_ASSERT_EQUAL(1, is_below_line(1, 1, 1, 3)); // Point is below line
  TEST_ASSERT_EQUAL(1,
                    is_below_line(1, 1, 1, 1)); // Point is exactly on the line
}

/* Test cases for is_above_line */
void test_is_above_line() {
  TEST_ASSERT_EQUAL(0, is_above_line(1, 1, 1, 2));  // Point is below line
  TEST_ASSERT_EQUAL(0, is_above_line(1, 1, 1, 1));  // Point is on the line
  TEST_ASSERT_EQUAL(1, is_above_line(2, 1, 1, 0));  // Point is above line
  TEST_ASSERT_EQUAL(1, is_above_line(1, 1, 1, -1)); // Point is above line
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_is_in_circle);
  RUN_TEST(test_is_below_line);
  RUN_TEST(test_is_above_line);
  UNITY_END();
  printf("Tests completed successfully!\n");
  return 0;
}
