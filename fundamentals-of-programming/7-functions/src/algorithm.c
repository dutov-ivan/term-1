#include "algorithm.h"
#include <Block.h>

#include <math.h>

#define RELATIVE_EPSILON 1e-9

#include "io/utils.h"

#define MAX_BISECTOR_ITERATIONS 1000000
#define MAX_NEWTON_ITERATIONS 1000

EquationFunc yxTemplate(double y) {
  // Blocks are awesome for creating lambdas
  double (^f)(double) = ^double(double x) {
    double frac = 1.0 / x;
    return cos(y * frac) - 2.0 * sin(frac) + frac;
  };
  return Block_copy(f);
}

EquationFunc ylnxTemplate(double y) {
  double (^f)(double) = ^double(double x) {
    double ln = log(x);
    return y * ln + sin(ln) - cos(ln);
  };
  return Block_copy(f);
}

double calculateDerivative(double x, EquationFunc func) {
  return (func(x + RELATIVE_EPSILON) - func(x)) / RELATIVE_EPSILON;
}

double bisectorMethod(double start, double end, EquationFunc func,
                      double precision) {
  int iteration = 0;

  if (func(start) * func(end) > 0.0) {
    handleError("Половинний метод не гарантує знаходження кореня, якщо "
                "значення функцій у межах має однаковий знак");
    return INFINITY;
  }
  double x;

  do {
    x = (start + end) / 2.0;

    double f_mid = func(x);
    if (fabs(f_mid) < precision) {
      return x;
    }

    if (func(start) * f_mid > 0.0) {
      start = x;
    } else {
      end = x;
    }

    iteration++;
    if (iteration > MAX_BISECTOR_ITERATIONS) {
      showWarning("Половинному методу не вдалося зійтися за %d ітерацій.",
                  MAX_BISECTOR_ITERATIONS);
      return INFINITY;
    }
  } while (fabs(end - start) > precision);

  return x;
}

double newtonMethod(double start, double end, EquationFunc func,
                    double precision) {
  int iteration = 0;
  double x = start;
  double delta;
  do {
    double derivative = calculateDerivative(x, func);
    if (fabs(derivative) < 1e-20) { // Avoid division by zero
      showWarning("Похідна функції занадто близька до нуля: %.lg", derivative);
      return INFINITY;
    }

    delta = func(x) / derivative;
    x -= delta;

    iteration++;
    if (iteration > MAX_NEWTON_ITERATIONS) {
      showWarning("Методу Ньютона не вдалося зійтись за %d ітерацій.",
                  MAX_NEWTON_ITERATIONS);
      return INFINITY;
    }

    if (fabs(delta) <= precision) {
      break;
    }

    if (isnan(delta) || isinf(delta)) {
      showWarning("Метод Ньютона зустрівся з проблемою: delta is NaN or Inf.");
      return INFINITY;
    }

  } while (true);

  if (x < start || x > end) {
    showWarning("Рішення вийшло за межі заданого інтервалу.");
  }

  return x;
}
