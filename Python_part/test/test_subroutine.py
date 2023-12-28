import sys

sys.path.append("src/")

from src.definition import Alloc, Setq
from src.expression import Expression
from src.subroutine import Call, DefSub

# def test_subroutine():
#     dispatch = {
#         "alloc": Alloc,
#         "setq": Setq,
#         "defsub": DefSub,
#         "call": Call,
#     }
#
#     # Test Subroutine: Define a subroutine to set x to 10, and then call it
#     # 10 x setq f defsub, f call
#     expression = Expression.from_program("10 x setq f defsub f call", dispatch)
#     env = {"x": 5}
#
#     expression.evaluate(env)
#
#     # After calling the subroutine, x should be set to 10
#     assert env["x"] == 10
