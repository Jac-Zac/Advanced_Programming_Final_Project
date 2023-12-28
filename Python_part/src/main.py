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
        "setv": Setv,
        "setq": Setq,
        "defsub": DefSub,
        "call": Call,
        "print": Print,
        "nop": Nop,
        "+": Addition,
        "*": Multiplication,
        "-": Subtraction,
        "%": Modulus,
        ">": Greater,
        ">=": GreaterEqual,
        "<": Less,
        "=": Equal,
        "!=": NotEqual,
        "if": IfInstruction,
        "while": While,
        "for": For,
        "prog2": Prog2,
        "prog3": Prog3,
        "prog4": Prog4,
    }

    expression = Expression.from_program(
        "nop x print prime if nop 0 0 != prime setq i x % 0 = if 1 x - 2 i for 0 0 = prime setq prime alloc prog4 100 2 x for",
        dispatch,
    )
    env = {}

    print(expression)
    expression.evaluate(env)

    # Check if the program correctly identifies prime numbers from 2 to 100
    # assert env["prime"] == [
    #     2,
    #     3,
    #     5,
    #     7,
    #     11,
    #     13,
    #     17,
    #     19,
    #     23,
    #     29,
    #     31,
    #     37,
    #     41,
    #     43,
    #     47,
    #     53,
    #     59,
    #     61,
    #     67,
    #     71,
    #     73,
    #     79,
    #     83,
    #     89,
    #     97,
    # ]
