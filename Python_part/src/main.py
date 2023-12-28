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
        "valloc": Valloc,
        "setq": Setq,
        "defsub": DefSub,
        "call": Call,
        "print": Print,
        "nop": Nop,
        "+": Addition,
        "*": Multiplication,
        "-": Subtraction,
        ">": Greater,
        ">=": GreaterEqual,
        "<": Less,
        "while": While,
        "for": For,
        "prog2": Prog2,
        "prog3": Prog3,
        "prog4": Prog4,
    }

    expression = Expression.from_program(
        "x 1 + x setq x 10 > while x alloc prog2", dispatch
    )
    env = {}

    expression.evaluate(env)
    print(env)

    assert env["x"] == 10
