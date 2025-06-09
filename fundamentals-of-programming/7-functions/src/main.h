#ifndef MAIN_H
#define MAIN_H

#include "algorithm.h"
#include "common/types.h"
#include "io/double.h"

#define YX_INDEX 0
#define YLNX_INDEX 1
#define MAX_ABS_DOUBLE_VALUE 1e6

#define OPTION_START 1

const DoubleRange DOUBLE_RANGE = {-MAX_ABS_DOUBLE_VALUE, MAX_ABS_DOUBLE_VALUE,
                                  true, true};
const DoubleRange YLNX_RANGE = {0, MAX_ABS_DOUBLE_VALUE, false, true};
const DoubleRange PRECISION_RANGE = {TOLERANCE_DOUBLE, 1.0, true, false};

#define EQUATION_COUNT 2
const char *EQUATION_PROMPT = "Оберіть функцію для розрахунку";
const IntRange EQUATION_CHOICE_RANGE = {
    OPTION_START, EQUATION_COUNT + OPTION_START - 1, true, true};
const char *EQUATION_DESCRIPTIONS[EQUATION_COUNT] = {
    "cos(y / x) - 2 * sin(1 / x) + 1 / x",
    "y * ln(x) + sin(ln(x)) - cos(ln(x))"};
const EquationTemplate EQUATION_FUNCTION_TEMPLATES[EQUATION_COUNT] = {
    &yxTemplate, &ylnxTemplate};
const char *EQUATION_TEMPLATES[EQUATION_COUNT] = {
    "cos(%g / %.*lf) - 2 * sin(1 / %.*lf) + 1 / %.*lf",
    "%g * ln(%.*lf) + sin(ln(%.*lf)) - cos(ln(%.*lf))",
};

#define METHOD_COUNT 2
const char *METHOD_PROMPT = "Оберіть метод для розрахунку функції";
const IntRange METHOD_CHOICE_RANGE = {
    OPTION_START, METHOD_COUNT + OPTION_START - 1, true, true};
const char *METHOD_DESCRIPTIONS[METHOD_COUNT] = {"Метод половинного ділення",
                                                 "Метод Ньютона"};
const EquationMethod METHODS[METHOD_COUNT] = {&bisectorMethod, &newtonMethod};

const char *OPTION_PROMPT = "Введіть опцію";

#endif
