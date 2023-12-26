import sys
import unittest

sys.path.append("../")

from src.expressions import *


class TestExpressions(unittest.TestCase):
    def test_expression_evaluation(self):
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
        result = expression.evaluate({"x": 3, "y": 7})
        expected_result = 0.84022932953024
        self.assertAlmostEqual(result, expected_result, places=5)


if __name__ == "__main__":
    unittest.main()
