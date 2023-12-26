#!/usr/bin/env python3

from expressions import *

if __name__ == "__main__":
    # Test the expression evaluation
    d = {
        "+": Addition,
        "*": Multiplication,
        "**": Power,
        "-": Subtraction,
        "/": Division,
        "1/": Reciprocal,
        "abs": AbsoluteValue,
    }
    example = "2 3 + x * 6 5 - / abs 2 ** y 1/ + 1/"
    expression = Expression.from_program(example, d)
    print(expression)
    result = expression.evaluate({"x": 3, "y": 7})
    print(result)  # Output attest:


# Output attest:
# 0.84022932953024

# YOU can add a method compile to compile into C code that can then be runned
