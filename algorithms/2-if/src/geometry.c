#include "geometry.h"

int is_in_circle(double x, double x1, double y, double y1, double r) {
  double x_center = x - x1;
  double y_center = y - y1;
  return x_center * x_center + y_center * y_center < r * r;
}

int is_below_line(double y, double k, double x, double b) {
  return y < k * x + b;
}

int is_above_line(double y, double k, double x, double b) {
  return y > k * x + b;
}
