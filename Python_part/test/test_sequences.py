import sys

import pytest

sys.path.append("src/")

from src.definition import *
from src.expression import Expression
from src.sequence import *

# Test cases
# Each test case includes: prog instruction, program string, expected result, and expected environment
test_cases = [
    ("Prog1", "5 x setq prog1", 5, {"x": 5}),
    ("Prog2", "x alloc 5 x setq 10 y setq prog2", 5, {"x": 5, "y": 10}),
    (
        "Prog3",
        "x alloc 5 x setq 10 y setq 20 z setq prog3",
        5,
        {"x": 5, "y": 10, "z": 20},
    ),
    (
        "Prog4",
        "x alloc 5 x setq 10 y setq 20 z setq 30 w setq prog4",
        5,
        {"x": 5, "y": 10, "z": 20, "w": 30},
    ),
]


@pytest.mark.parametrize(
    "prog_instruction, program, expected_result, expected_env", test_cases
)
def test_prog_operations(prog_instruction, program, expected_result, expected_env):
    dispatch = {
        "alloc": Alloc,
        "setq": Setq,
        prog_instruction.lower(): globals()[prog_instruction],
    }
    expression = Expression.from_program(program, dispatch)
    env = {}
    result = expression.evaluate(env)
    assert result == expected_result  # Check the result
    assert env == expected_env  # Check the environment


# Run the tests
if __name__ == "__main__":
    pytest.main()
