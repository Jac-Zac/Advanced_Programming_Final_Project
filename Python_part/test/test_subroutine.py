# Jacopo Zacchinga SM3201293
import sys

sys.path.append("src/")

from src.definition import Alloc, Setq
from src.expression import Expression
from src.sequence import Prog2
from src.subroutine import Call, DefSub


def test_subroutine():
    dispatch = {
        "alloc": Alloc,
        "setq": Setq,
        "defsub": DefSub,
        "call": Call,
        "prog2": Prog2,
    }

    expression = Expression.from_program("f call 10 x setq f defsub prog2", dispatch)

    env = {"x": 0}

    expression.evaluate(env)

    # After calling the subroutine, x should be set to 10
    assert env["x"] == 10
