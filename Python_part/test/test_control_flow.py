# Jacopo Zacchinga SM3201293
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
        "+": Addition,
    }

    # Test While loop: X is less then 10 we increase the value
    expression = Expression.from_program("1 x + x 10 < while", dispatch)
    env = {"x": 0}

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
    expression = Expression.from_program("sum i + sum setq 5 0 i for", dispatch)
    env = {"sum": 0}

    expression.evaluate(env)

    # After the loop, sum should be 10 (0+1+2+3+4)
    assert env["sum"] == 10
