#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <stdarg.h>
#include <stdbool.h>

bool isGreater(double b, va_list args);
bool isNotZero(double b, va_list args);
bool isNotContainZero(double b, va_list args);
bool isGreaterAndNotContainZero(double b, va_list args);

#endif // !VALIDATORSH
