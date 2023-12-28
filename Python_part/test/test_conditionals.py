from src.condition import IfInstruction
from src.expression import Constant, Expression


def test_if_operation():
    dispatch = {"if": IfInstruction, "const": Constant}
    # Test with a truthy condition (non-zero number)
    expression = Expression.from_program("10 20 1 if", dispatch)
    env = {}
    result = expression.evaluate(env)
    assert (
        result == 20
    )  # Should return the result of 'if-yes' branch (20) for truthy condition

    # Test with a falsy condition (0)
    expression = Expression.from_program("10 20 0 if", dispatch)
    env = {}
    result = expression.evaluate(env)
    assert (
        result == 10
    )  # Should return the result of 'if-no' branch (10) for falsy condition
