import sys

sys.path.append("src/")

from src.control_flow import For, While
from src.definition import Alloc, Setq
from src.expression import Expression
from src.instruction import *  # Importing all instruction classes
from src.sequence import *
from src.subroutine import Call, DefSub


def test_code_examples():
    dispatch = {
        "alloc": Alloc,
        "setq": Setq,
        "defsub": DefSub,
        "call": Call,
        "print": Print,
        "nop": Nop,
        "+": Addition,
        "*": Multiplication,
        ">": Greater,
        "while": While,
        "for": For,
        "prog2": Prog2,
        "prog4": Prog4,
    }

    # Test Example 1: While loop to increment x until it's greater than 10
    expression1 = Expression.from_program(
        "x 1 + x setq x 10 > while x alloc prog2", dispatch
    )

    env1 = {}

    expression1.evaluate(env1)

    # After the loop, x should be 10
    assert env1["x"] == 10

    # Test Example 2: For loop to initialize and print a vector
    expression2 = Expression.from_program(
        "v print i i * i v setv prog2 10 0 i for 10 v valloc prog2", dispatch
    )
    env2 = {}

    expression2.evaluate(env2)

    # Check if the vector 'v' is correctly initialized and printed
    assert env2["v"] == [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]

    # Test Example 3: Define a subroutine, call it, and print the variable
    expression3 = Expression.from_program(
        "x print f call x alloc x 4 + x setq f defsub prog4", dispatch
    )
    env3 = {}

    expression3.evaluate(env3)

    # After calling the subroutine, x should be incremented to 14
    assert env3["x"] == 14

    # Test Example 4: Divisor finder program
    expression4 = Expression.from_program(
        "nop i print i x % 0 = if 1000 2 i for 783 x setq x alloc prog3", dispatch
    )
    env4 = {}

    expression4.evaluate(env4)

    # Check if the program correctly finds divisors of 783
    assert env4["x"] == 783

    # Test Example 5: Prime number checker program
    expression5 = Expression.from_program(
        "nop x print prime if nop 0 0 != prime setq i x % 0 = if 1 x - 2 i for 0 0 = prime setq prime alloc prog4 100 2 x for",
        dispatch,
    )
    env5 = {}

    expression5.evaluate(env5)

    # Check if the program correctly identifies prime numbers from 2 to 100
    assert env5["prime"] == [
        2,
        3,
        5,
        7,
        11,
        13,
        17,
        19,
        23,
        29,
        31,
        37,
        41,
        43,
        47,
        53,
        59,
        61,
        67,
        71,
        73,
        79,
        83,
        89,
        97,
    ]


if __name__ == "__main__":
    test_code_examples()
