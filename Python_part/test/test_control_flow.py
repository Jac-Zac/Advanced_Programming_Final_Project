import sys

sys.path.append("src/")

from src.control_flow import *
from src.definition import (  # Assuming these are defined for setting and allocating variables
    Alloc, Setq)
from src.expression import Expression
from src.instruction import *


def test_while_control_flow():
    dispatch = {
        "alloc": Alloc,
        "setq": Setq,
        "while": While,
        "<": Less,
        "-": Subtraction,
    }

    # Test While loop: Decrement x until it is less than 1
    expression = Expression.from_program("x 1 < 1 x - x setq while", dispatch)
    env = {"x": 3}

    expression.evaluate(env)

    # After the loop, x should be 0
    assert env["x"] == 0


def test_for_control_flow():
    dispatch = {
        "alloc": Alloc,
        "setq": Setq,
        "for": For,  # Adding the For class to the dispatch
        "+": Addition,
        "=": Equal,
    }

    # Test For loop: Sum numbers from 0 to 4
    # for i 0 5 (sum i + sum setq)
    expression = Expression.from_program("i 0 5 sum i + sum setq for", dispatch)
    env = {"sum": 0}

    expression.evaluate(env)

    # After the loop, sum should be 10 (0+1+2+3+4)
    assert env["sum"] == 10
