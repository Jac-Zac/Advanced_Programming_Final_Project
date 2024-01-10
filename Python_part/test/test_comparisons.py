# Jacopo Zacchinga SM3201293
import sys

import pytest

sys.path.append("src/")

from src.expression import Expression
from src.instruction import *

OPERATOR_MAPPING = {
    "<": Less,
    "<=": LessEqual,
    ">": Greater,
    ">=": GreaterEqual,
    "==": Equal,
    "!=": NotEqual,
}


@pytest.mark.parametrize(
    "operator_class, operator_symbol, left, right, expected_result",
    [
        (Less, ">", 2, 3, True),
        (LessEqual, ">=", 3, 3, True),
        (Greater, "<", 4, 3, True),
        (GreaterEqual, "<=", 3, 3, True),
        (Equal, "==", 5, 5, True),
        (NotEqual, "!=", 5, 4, True),
    ],
)
def test_individual_comparison_operators(
    operator_class, operator_symbol, left, right, expected_result
):
    expression_str = f"{left} {right} {operator_symbol}"
    expression = Expression.from_program(expression_str, OPERATOR_MAPPING)
    env = {}  # Empty environment, as we're not using variables in this test
    assert expression.evaluate(env) == expected_result


def test_complex_operators():
    example = "2 3 <= x !="
    expression = Expression.from_program(example, OPERATOR_MAPPING)
    expected_str = "(!= x (<= 3 2))"
    assert str(expression) == expected_str
    env = {"x": 4, "y": 5}
    expected_result = True
    result = expression.evaluate(env)
    assert result == expected_result
