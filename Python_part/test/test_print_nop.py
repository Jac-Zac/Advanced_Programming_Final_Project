from src.definition import *
from src.expression import Expression
from src.instruction import *


def test_print_nop_operations():
    dispatch = {
        "alloc": Alloc,
        "setq": Setq,
        "print": Print,  # Adding the Print class to the dispatch
        "nop": Nop,  # Adding the Nop class to the dispatch
        "+": Addition,
        "=": Equal,
    }

    # Test Print: Evaluate and print an expression
    # x 5 setq x print
    expression = Expression.from_program("x print 5 x setq ", dispatch)
    env = {}

    result = expression.evaluate(env)

    # Ensure that 'x' is set to 5, and the result is 5 (printed)
    assert env["x"] == 5
    assert result == 5


if __name__ == "__main__":
    test_print_nop_operations()
