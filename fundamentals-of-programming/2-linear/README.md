# Prostoangle calculator

This program computes various geometric properties of a triangle, including its perimeter, area, heights, medians, and bisectors. It interacts with the user to accept triangle side lengths and desired precision for the results, performing calculations based on user input.

## Features
The program can calculate:
- The perimeter of the triangle
- The area of the triangle
- The heights relative to each side of the triangle
- The medians relative to each side
- The bisectors relative to each side

### Limitations:
- Maximum side length: `MAX_SIDE_LENGTH`
- Minimum side length: `MIN_SIDE_LENGTH`
- Maximum precision (number of decimal places): `MAX_GUARANTEED_PRECISION`
- Minimum precision: `MIN_GUARANTEED_PRECISION`
- The length of the side input should not exceed `NUMBER_LENGTH_LIMIT` characters.

## Compilation

To compile the program, use the following command:

```bash
gcc -o triangle_calculator main.c triangle.c -lm
```
OR
```bash
make clean && make
```
If you want tests to work, make sure to `#define UNITY_INCLUDE_DOUBLE` before `#include "unity_internals.h"`

## Usage

Run the program from the command line:

```bash
./triangle_calculator [options]
```

### Options:

- `-q`, `--quiet`: Suppresses extra output from the program.
- `-r`, `--repeat`: Repeats the program after completing calculations until interrupted by the user.
- `-h`, `--help`: Displays help information and exits.

### Example:

```bash
./triangle_calculator -r -q
```

This command runs the program in quiet mode and repeats after each calculation.

## Input

The program will prompt you for the lengths of the sides of the triangle (`a`, `b`, and `c`). You will also need to specify the desired precision for the results (number of decimal places).

**Note:** The program expects side lengths to be positive real numbers. It will also check if the provided side lengths form a valid triangle.

### Example Interaction:

```bash
Введіть, будь ласка, довжину сторони a: 3
Введіть, будь ласка, довжину сторони b: 4
Введіть, будь ласка, довжину сторони c: 5
Введіть кількість знаків після коми: 2
```

After entering valid values, the program will display the calculated results for the perimeter, area, heights, medians, and bisectors with the specified precision.

## Exiting

To exit the program, press `Ctrl+C`.

## Additional Information

- The program will increase the precision automatically if required to differentiate between lengths in the output.
- Input validation is performed to ensure correct and valid values for both triangle sides and precision.

## License

This project is closed-source and is NOT available for modification and redistribution.
