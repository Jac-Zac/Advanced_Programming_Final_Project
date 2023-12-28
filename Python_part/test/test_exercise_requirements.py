import sys

sys.path.append("src/")

from src.expression import Expression
from src.instruction import Instruction
from src.operation import *


def test_exercise_requirements():
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
    expected_str = "(1/ (+ (1/ y) (** 2 (abs (/ (- 5 6) (* x (+ 3 2)))))))"

    assert str(expression) == expected_str

    result = expression.evaluate({"x": 3, "y": 7})
    expected_result = 0.84022932953024
    assert (
        abs(result - expected_result) < 1e-5
    )  # Equivalent to almostEqual with 5 decimal places
