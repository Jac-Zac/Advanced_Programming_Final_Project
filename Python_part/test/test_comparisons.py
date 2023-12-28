import sys

sys.path.append("src/")

from src.comparison import *
from src.expression import Expression
from src.operation import *


def test_comparison_operators():
    """
    Quick test for everything since the code is quite simple and repetitive
    """
    d = {
        "<": Less,
        "<=": LessEqual,
        ">": Greater,
        ">=": GreaterEqual,
        "==": Equal,
        "!=": NotEqual,
    }

    # Example expression using comparison operators
    example = "2 3 <= x !="
    expression = Expression.from_program(example, d)

    expected_str = "(!= x (<= 3 2))"

    assert str(expression) == expected_str

    # Environment for the variables
    env = {"x": 4, "y": 5}

    # Expected result based on the given environment
    expected_result = True

    # Evaluate the expression and compare the result
    result = expression.evaluate(env)
    assert result == expected_result
