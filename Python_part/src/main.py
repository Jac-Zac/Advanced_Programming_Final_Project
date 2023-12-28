#!/usr/bin/env python3

from definition import *
from expression import *
from operation import *

if __name__ == "__main__":
    dispatch = {"valloc": Valloc}
    expression = Expression.from_program("5 y valloc", dispatch)
    env = {}
    expression.evaluate(env)

# YOU can add a method compile to compile into C code that can then be runned
