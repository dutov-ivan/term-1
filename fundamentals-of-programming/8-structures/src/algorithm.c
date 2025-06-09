#include "algorithm.h"

Complex divideComplex(Complex *numerator, Complex *denominator) {
  Complex res;
  // Creating variables to make code more readable
  double a = numerator->Re, b = numerator->Im, c = denominator->Re,
         d = denominator->Im;
  double frac = 1.0 / (c * c + d * d);

  res.Re = (a * c + b * d) * frac;
  res.Im = (b * c - a * d) * frac;

  return res;
}

// INFO: The naming of methods starts on the schematic diagram from the left top
// corner clockwise

Complex solveRLC(const CircuitParams *params) {
  double L = params->L, C = params->C, w = params->w, R = params->R1;

  Complex numerator, denominator;
  numerator.Re = L / C;
  numerator.Im = -R / (w * C);
  denominator.Re = R;
  denominator.Im = w * L - 1.0 / (w * C);
  return divideComplex(&numerator, &denominator);
}

Complex solveRCL(const CircuitParams *params) {
  double L = params->L, C = params->C, w = params->w, R = params->R1;

  Complex numerator, denominator;
  numerator.Re = L / C;
  numerator.Im = R / (w * C);
  denominator.Re = R;
  denominator.Im = w * L - 1.0 / (w * C);
  return divideComplex(&numerator, &denominator);
}

Complex solveR2CLR1(const CircuitParams *params) {
  double L = params->L, C = params->C, w = params->w, R1 = params->R1,
         R2 = params->R2;

  Complex numerator, denominator;
  numerator.Re = R1 * R2;
  numerator.Im = R1 * (w * L - 1.0 / (w * C));
  denominator.Re = R1 + R2;
  denominator.Im = w * L - 1.0 / (w * C);
  return divideComplex(&numerator, &denominator);
}

Complex solveR2LCR1(const CircuitParams *params) {
  double L = params->L, C = params->C, w = params->w, R1 = params->R1,
         R2 = params->R2;

  Complex numerator, denominator;
  numerator.Re = R1 * R2 + L / C;
  numerator.Im = w * L * R1 - R2 / (w * C);
  denominator.Re = R1 + R2;
  denominator.Im = w * L - 1.0 / (w * C);
  return divideComplex(&numerator, &denominator);
}
