#ifndef MAIN_H
#define MAIN_H

#include "algorithm.h"
#include "common/types.h"
#include <float.h>

#define DECIMAL_PLACES DBL_MAX_DIGITS
#define COLUMN_WIDTH 10
#define R2CLR1_INDEX 2 // index of first circuit with two resistors
#define R2LCR1_INDEX 3 // index of the second circuit with two resistors
#define CIRCUIT_COUNT 4

const DoubleRange DEFAULT_RANGE = {1e-6, 1e6, true, true};

const char *CIRCUIT_DESCRIPTIONS[CIRCUIT_COUNT] = {
    "\n⦿──┬──R──L──┬──⦿"
    "\n   │        │"
    "\n   └───C────┘",

    "\n⦿──┬──R──C──┬──⦿"
    "\n   │        │"
    "\n   └───L────┘",

    "\n⦿──┬──R2──C──┬──⦿"
    "\n   R1        │"
    "\n   └────L────┘",

    "\n⦿──┬──R2──┐"
    "\n   R1     │"
    "\n   │      │"
    "\n   C      │"
    "\n⦿──┴───L──┘",
};

const IntRange CIRCUIT_CHOICE_RANGE = {1, CIRCUIT_COUNT, true, true};
const char *CIRCUIT_PROMPT =
    "Оберіть електричне коло, комплексний опір якого ви хочете обчислити";
const CircuitSolver CIRCUIT_SOLVERS[CIRCUIT_COUNT] = {solveRLC, solveRCL,
                                                      solveR2CLR1, solveR2LCR1};

#endif
