from src.condition import IfInstruction
from src.expression import Constant, Expression
from src.instruction import Greater, Less


def test_if_operation():
    dispatch = {"if": IfInstruction, "const": Constant}
    # Test with a truthy condition (non-zero number)
    expression1 = Expression.from_program("10 20 1 if", dispatch)
    env = {"x": 1}
    result1 = expression1.evaluate(env)
    assert (
        result1 == 20
    )  # Should return the result of 'if-yes' branch (20) for truthy condition

    # Test with a falsy condition (0)
    expression2 = Expression.from_program("10 20 0 if", dispatch)
    result2 = expression2.evaluate(env)
    assert (
        result2 == 10
    )  # Should return the result of 'if-no' branch (10) for falsy condition

    # You can also test something a bit more elaborate like this with Greater
    # expression = Expression.from_program("10 20 x 10 > if", dispatch)
