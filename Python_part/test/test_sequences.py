import sys

sys.path.append("src/")

from src.definition import *
from src.expression import Expression
from src.sequence import *


def test_prog1():
    dispatch = {"alloc": Alloc, "setq": Setq, "prog1": Prog1}
    # Test with a single setq operation
    expression = Expression.from_program("5 x setq prog1", dispatch)
    env = {}
    result = expression.evaluate(env)

    assert result == 5  # The result should be the value of the setq operation
    assert env["x"] == 5  # x should be set to 5


def test_prog2():
    dispatch = {"alloc": Alloc, "setq": Setq, "prog2": Prog2}
    expression = Expression.from_program("x alloc 5 x setq 10 y setq prog2", dispatch)
    env = {}
    result = expression.evaluate(env)
    assert result == 5  # The result should be the value of the first expression
    assert env["x"] == 5
    assert env["y"] == 10


def test_prog3():
    dispatch = {"alloc": Alloc, "setq": Setq, "prog3": Prog3}
    expression = Expression.from_program(
        "x alloc 5 x setq 10 y setq 20 z setq prog3", dispatch
    )
    env = {}
    result = expression.evaluate(env)
    assert result == 5  # The result should be the value of the first expression
    assert env["x"] == 5  # x should be set to 5
    assert env["y"] == 10  # y should be set to 10
    assert env["z"] == 20  # z should be set to 20


def test_prog4():
    dispatch = {"alloc": Alloc, "setq": Setq, "prog4": Prog4}
    expression = Expression.from_program(
        "x alloc 5 x setq 10 y setq 20 z setq 30 w setq prog4", dispatch
    )
    env = {}
    result = expression.evaluate(env)
    assert result == 5  # The result should be the value of the first expression
    assert env["x"] == 5  # x should be set to 5
    assert env["y"] == 10  # y should be set to 10
    assert env["z"] == 20  # z should be set to 20
    assert env["w"] == 30  # w should be set to 30


# def test_variable_definition_and_manipulation():
#     dispatch = {
#         "alloc": Alloc,
#         "valloc": Valloc,
#         "setq": Setq,
#         "setv": Setv,
#     }
#
#     # Testing variable allocation, array allocation, and setting values
#     example = "10 x alloc 5 y valloc 3 z setq 4 2 y setv"
#     expression = Expression.from_program(example, dispatch)
#     env = {}
#     expression.evaluate(env)
#     assert env["x"] == 0  # x should be initialized to 0
#     assert (
#         len(env["y"]) == 5 and env["y"][0] == 0
#     )  # y should be an array of 5 elements, all zeros
#     assert env["z"] == 3  # z should be set to 3
#     assert env["y"][2] == 4  # 3rd element (index 2) of y should be set to 4
#

# def test_sequences():
#     dispatch = {
#         # Include all necessary operation classes
#     }
#     example = "1 2 + 3 4 * prog2"
#     expression = Expression.from_program(example, dispatch)
#     expected_result = 3  # Since the first expression (1 + 2) evaluates to 3
#     result = expression.evaluate({})
#     assert result == expected_result
#
# def test_conditional_and_loops():
#     dispatch = {
#         # Include all necessary operation classes
#     }
#     example = "5 3 > 'yes' 'no' if"
#     expression = Expression.from_program(example, dispatch)
#     expected_result = 'yes'  # Since 5 > 3 is True
#     result = expression.evaluate({})
#     assert result == expected_result
#
#     # Similar tests for while and for loops
#
# def test_subroutines():
#     dispatch = {
#         # Include all necessary operation classes
#     }
#     example = "4 x setq x 1 + x setq 'increment' defsub 'increment' call"
#     expression = Expression.from_program(example, dispatch)
#     env = {}
#     result = expression.evaluate(env)
#     assert env['x'] == 5  # x initially set to 4, then incremented
#
#
# def test_additional_functionalities():
#     dispatch = {
#         # Include all necessary operation classes
#     }
#     example = "10 print nop"
#     expression = Expression.from_program(example, dispatch)
#     result = expression.evaluate({})
#     # Testing print and nop might require capturing stdout or specific implementation checks
