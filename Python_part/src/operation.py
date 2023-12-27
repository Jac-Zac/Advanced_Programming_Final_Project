# Jacopo Zacchinga ...

from abc import abstractmethod
from typing import Any, Dict, List

from expression import Expression


class Operation(Expression):
    arity: int = 0

    def __init__(self, args: List[Expression]):
        self.args = args

    def evaluate(self, env: Dict[str, Any]) -> Any:
        """
        Evaluate the operation in a given Expression

        This method processes each argument of the operation by evaluating them recursively until we meat a variable or a constant, and then applies the operation defined in the 'op' method to these evaluated arguments.

        Parameters:
        env (Dict[str, Any]): The environment for variable evaluation.

        Returns:
        Any: The result of the evaluation.
        """
        evaluated_args = [arg.evaluate(env) for arg in self.args]
        return self.op(*evaluated_args)

    @abstractmethod
    def op(self, *args) -> Any:
        pass

    def __str__(self) -> str:
        args_str = " ".join(str(arg) for arg in self.args)
        op_name = self.get_op_name()
        return f"({op_name} {args_str})"

    def get_op_name(self) -> str:
        for name, cls in self.dispatch.items():
            if cls is self.__class__:
                return name
        return "unknown"


class TernaryOp(Operation):
    arity = 3

    def op(self, a, b, c) -> Any:
        pass


class BinaryOp(Operation):
    arity = 2

    def op(self, a, b) -> Any:
        pass


class UnaryOp(Operation):
    arity = 1

    def op(self, a) -> Any:
        pass


############################## Mathematical Operations ##############################


class Addition(BinaryOp):
    def op(self, a, b):
        return a + b


class Subtraction(BinaryOp):
    def op(self, a, b):
        return a - b


class Multiplication(BinaryOp):
    def op(self, a, b):
        return a * b


class Division(BinaryOp):
    def op(self, a, b):
        if b == 0:
            raise ValueError("Division by zero")
        return a / b


class Power(BinaryOp):
    def op(self, a, b):
        return a**b


class Modulus(BinaryOp):
    def op(self, a, b):
        return a % b


class Reciprocal(UnaryOp):
    def op(self, a):
        if a == 0:
            raise ValueError("Division by zero")
        return 1 / a


class AbsoluteValue(UnaryOp):
    def op(self, a):
        return abs(a)


############################## Comparison Operations ##############################


class ComparisonOp(BinaryOp):
    """
    Base class for comparison operations.
    """


class Greater(ComparisonOp):
    def op(self, a, b):
        return a > b


class GreaterEqual(ComparisonOp):
    def op(self, a, b):
        return a >= b


class Equal(ComparisonOp):
    def op(self, a, b):
        return a == b


class NotEqual(ComparisonOp):
    def op(self, a, b):
        return a != b


class Less(ComparisonOp):
    def op(self, a, b):
        return a < b


class LessEqual(ComparisonOp):
    def op(self, a, b):
        return a <= b
