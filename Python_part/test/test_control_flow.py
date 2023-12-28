import sys

sys.path.append("src/")

from src.comparison import *
from src.control_flow import While
from src.definition import (  # Assuming these are defined for setting and allocating variables
    Alloc, Setq)
from src.expression import Expression


def test_while_control_flow():
    dispatch = {
        "alloc": Alloc,
        "setq": Setq,
        "while": While,
        "<": Less,  # Assuming Less is a comparison operation
    }

    # Test While loop: Decrement x until it is less than 1
    expression = Expression.from_program("x 1 < 1 x - x setq while", dispatch)
    env = {"x": 3}

    expression.evaluate(env)

    # After the loop, x should be 0
    assert env["x"] == 0
