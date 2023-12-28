import sys

sys.path.append("src/")

from src.definition import *
from src.expression import Expression
from src.instruction import Addition
from src.sequence import *


def test_prog1():
    dispatch = {"alloc": Alloc, "setq": Setq, "prog1": Prog1, "+": Addition}
    # Test with a single setq operation
    expression = Expression.from_program("5 10 + prog1", dispatch)
    env = {}
    result = expression.evaluate(env)

    assert result == 15  # the result of the first expression should be what we expect


def test_prog2():
    dispatch = {"alloc": Alloc, "setq": Setq, "prog2": Prog2, "+": Addition}
    expression = Expression.from_program("3 2 + 10 x setq prog2", dispatch)
    env = {"x": 0}
    result = expression.evaluate(env)
    assert result == 5  # The result should be the value of the first expression
    assert env["x"] == 10


def test_prog3():
    dispatch = {"alloc": Alloc, "setq": Setq, "prog3": Prog3, "+": Addition}
    expression = Expression.from_program("3 2 + x setqx alloc prog3", dispatch)
    env = {}
    result = expression.evaluate(env)
    assert result == 5  # The result should be the value of the first expression
    assert env["x"] == 10


def test_prog4():
    dispatch = {"alloc": Alloc, "setq": Setq, "prog4": Prog4, "+": Addition}
    expression = Expression.from_program(
        "3 2 + 10 x setq 15 y setq 20 z setq prog4", dispatch
    )
    env = {"x": 0, "y": 0, "z": 0}
    result = expression.evaluate(env)
    assert result == 5
    assert env["x"] == 10
    assert env["y"] == 15
    assert env["z"] == 20
