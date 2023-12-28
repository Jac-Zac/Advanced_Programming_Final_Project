import sys

import pytest

sys.path.append("src/")

from src.expression import Expression
from src.instruction import *

# # Test data for individual comparison operators
# @pytest.mark.parametrize(
#     "operator_class, operator_symbol, left, right, expected_result",
#     [
#         (Less, "<", 2, 3, True),
#         (LessEqual, "<=", 3, 3, True),
#         (Greater, ">", 4, 3, True),
#         (GreaterEqual, ">=", 3, 3, True),
#         (Equal, "==", 5, 5, True),
#         (NotEqual, "!=", 5, 4, True),
#     ],
# )
# def test_individual_comparison_operators(
#     operator_class, operator_symbol, left, right, expected_result
# ):
#     # Construct the expression string
#     expression_str = f"{left} {right} {operator_symbol}"
#
#     # Create a dispatch dictionary
#     dispatch = {operator_symbol: operator_class}
#
#     # Parse the expression
#     expression = Expression.from_program(expression_str, dispatch)
#
#     # Evaluate the expression and assert the result
#     env = {}  # Empty environment, as we're not using variables in this test
#     assert expression.evaluate(env) == expected_result
#

# # Run the tests
# if __name__ == "__main__":
#     pytest.main()


def test_complex_operators():
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
