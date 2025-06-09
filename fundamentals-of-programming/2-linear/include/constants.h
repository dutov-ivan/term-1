// Side lengths
#define MAX_SIDE_LENGTH (long double)1e6
#define MIN_SIDE_LENGTH (long double)1e-3

// Precision settings
#define MAX_GUARANTEED_PRECISION 15 // Maximum precision for guaranteed results
#define MAX_ACTUAL_PRECISION 50     // Maximum possible precision
#define MIN_GUARANTEED_PRECISION 1  // Minimum precision

// General tolerance for floating-point comparisons
#define TOLERANCE (long double)1e-15

// Max length of number (whole part + fraction + decimal point)
#define NUMBER_LENGTH_LIMIT (7 + MAX_GUARANTEED_PRECISION + 1)

// Triangle properties
#define SIDE_COUNT 3 // A triangle has 3 sides
#define PROPERTY_COUNT                                                         \
  (SIDE_COUNT * 3 + 2) // 3 heights, 3 medians, 3 bisectors, perimeter, area

// Utility macros
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
