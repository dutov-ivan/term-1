#ifndef ALGORITHM_H
#define ALGORITHM_H

typedef struct {
  double Re;
  double Im;
} Complex;

typedef struct {
  double L, C, w;
  double R1, R2; // Use R1 for single resistance cases
} CircuitParams;

typedef Complex (*CircuitSolver)(const CircuitParams *);

Complex solveRLC(const CircuitParams *params);
Complex solveRCL(const CircuitParams *params);
Complex solveR2CLR1(const CircuitParams *params);
Complex solveR2LCR1(const CircuitParams *params);

#endif // !ALGORITHM_H
