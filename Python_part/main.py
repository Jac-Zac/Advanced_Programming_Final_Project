#!/usr/bin/env python3

from src import *

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
        "if": If,
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
