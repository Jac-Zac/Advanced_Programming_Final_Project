# Jacopo Zacchinga SM3201293
import sys

sys.path.append("src/")

from src.definition import *
from src.expression import Expression


def test_alloc():
    dispatch = {"alloc": Alloc}
    expression = Expression.from_program("x alloc", dispatch)
    env = {}
    expression.evaluate(env)
    assert env["x"] == 0  # x should be initialized to 0


def test_valloc():
    dispatch = {"valloc": Valloc}
    expression = Expression.from_program("5 y valloc", dispatch)
    env = {}
    expression.evaluate(env)
    assert len(env["y"]) == 5 and all(elem == 0 for elem in env["y"])


def test_setq():
    dispatch = {"alloc": Alloc, "setq": Setq}
    expression = Expression.from_program("x alloc 4 x setq", dispatch)
    env = {}
    expression.evaluate(env)
    assert env["x"] == 4  # x should be set to 4


def test_setv():
    # You can also do something like this bu you also need prog
    # expression = Expression.from_program("12 1 y setv 10 y valloc prog2", dispatch)
    # env = {"x": 1}
    # expression.evaluate(env)
    # assert env["y"][1] == 12

    dispatch = {"setv": Setv}
    expression = Expression.from_program("2 1 y setv", dispatch)
    env = {"y": [0, 0, 0, 0, 0]}

    expression.evaluate(env)
    assert env["y"][1] == 2  # The second element of y should be set to 2
