#!/usr/bin/env python3

from condition import IfInstruction
from control_flow import *
from definition import *
from expression import *
from instruction import *
from sequence import *
from subroutine import *

if __name__ == "__main__":
    dispatch = {
        "alloc": Alloc,
        "setq": Setq,
        "defsub": DefSub,
        "call": Call,
        "print": Print,
        "nop": Nop,
        "+": Addition,
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
    env = {}

    print(expression1)

    expression1.evaluate(env)

    print(env)

    assert env["x"] == 11

    # After the loop, x should be 11
