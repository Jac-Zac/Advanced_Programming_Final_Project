#!/usr/bin/env python3

from expression import *
from operation import *
from variable_definition import *

if __name__ == "__main__":
    # Example dispatch dictionary


# def test_program_evaluation():
#     d = {
#         "+": Addition,
#         "*": Multiplication,
#         "**": Power,
#         "-": Subtraction,
#         "/": Division,
#         "1/": Reciprocal,
#         "abs": AbsoluteValue,
#     }
#     example = """
#         nop
#         x print
#         prime if
#         nop
#         0 0 != prime setq
#         i x % 0 = if
#         1 x - 2 i for
#         0 0 = prime setq
#         prime alloc
#         prog4
#         100 2 x for
#     """
#
#     expression = Expression.from_program(example, d)
#     expected_str = "(1/ (+ (1/ y) (** 2 (abs (/ (- 5 6) (* x (+ 3 2)))))))"
#
#     assert str(expression) == expected_str
#
#     result = expression.evaluate({"x": 3, "y": 7})
#     expected_result = 0.84022932953024
#     assert (
#         abs(result - expected_result) < 1e-5
#     )  # Equivalent to almostEqual with 5 decimal places

# YOU can add a method compile to compile into C code that can then be runned
