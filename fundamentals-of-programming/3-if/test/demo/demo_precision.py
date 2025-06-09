from decimal import Decimal, getcontext
import csv

# Set precision to 100 decimal places
getcontext().prec = 100

def approximate(base: str, power: str) -> Decimal:
    base_decimal = Decimal(base)
    power_decimal = Decimal(power)

    if power_decimal == 0:
        raise ValueError("Power cannot be zero (division by zero).")

    # Handle negative bases
    if base_decimal < 0:
        # For even roots, raise an error since we cannot compute the root of a negative number
        if power_decimal % 2 == 0:
            raise ValueError("Cannot take even root of a negative number.")
        
        # Calculate using absolute value for negative base with odd roots
        result = (-base_decimal) ** (Decimal(1) / power_decimal)
        return -result  # Return the negative of the result since we took the absolute value

    # Calculate the root normally for positive bases
    return base_decimal ** (Decimal(1) / power_decimal)

input_list = [
    ["17.8", "14", "1.228331854041281"],
    ["-73", "-3", "-0.239272275595"],  # Example of negative base
    ["234.786432", "-123", "0.956590879450327"],
    ["512", "9", "2.00000"]
]

results = []

for case in input_list:
    try:
        precise = approximate(case[0], case[1])
        predicted = Decimal(case[-1])
        error = abs(predicted - precise)

        # Format to 15 decimal places
        formatted_precise = f"{precise:.15f}"
        formatted_error = f"{error:.15f}"
        formatted_predicted = f"{predicted:.15f}"

        results.append([case[0], case[1], formatted_predicted, formatted_precise, formatted_error])
    except ValueError as e:
        print(f"Error calculating root for {case[0]}^{case[1]}: {e}")

CSV_FILENAME = 'precision.csv'
with open(CSV_FILENAME, mode='w', newline='', encoding='utf-8') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(["x", "k", "predicted", "precise", "error"])
    for row in results:
        writer.writerow(row)

print(f"Results have been written to {CSV_FILENAME}.")
