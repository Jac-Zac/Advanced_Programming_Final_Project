#!/usr/bin/env python3

from definition import *
from expression import *
from operation import *
from sequence import *

if __name__ == "__main__":
    dispatch = {"alloc": Alloc, "setq": Setq, "prog2": Prog2}
    expression = Expression.from_program("x alloc 5 x setq 10 y setq prog2", dispatch)
    env = {}
    print(expression.evaluate(env))

# YOU can add a method compile to compile into C code that can then be runned
