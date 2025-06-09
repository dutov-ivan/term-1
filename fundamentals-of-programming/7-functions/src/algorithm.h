#ifndef ALGORITHM_H
#define ALGORITHM_H

typedef double (^EquationFunc)(double x);

typedef double (*EquationMethod)(double start, double end, EquationFunc func,
                                 double precision);
typedef EquationFunc (*EquationTemplate)(double);

EquationFunc yxTemplate(double y);
EquationFunc ylnxTemplate(double y);
double newtonMethod(double start, double end, EquationFunc func,
                    double precision);
double bisectorMethod(double start, double end, EquationFunc func,
                      double precision);

#endif // !ALGORITHM_H
