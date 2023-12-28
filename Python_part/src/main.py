#!/usr/bin/env python3

from condition import IfInstruction
from control_flow import *
from definition import *
from expression import *
from sequence import *

if __name__ == "__main__":
    dispatch = {"if": IfInstruction, "const": Constant}
    # Test with a truthy condition (non-zero number)
    expression = Expression.from_program("10 20 1 if", dispatch)
    env = {}
    print(expression)
    result = expression.evaluate(env)
    assert (
        result == 20
    )  # Should return the result of 'if-yes' branch (20) for truthy condition


# YOU can add a method compile to compile into C code that can then be runned
