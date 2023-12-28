# Jacopo Zacchigna
from abc import abstractmethod
from typing import Any, Dict, List

from expression import Expression
from utils.mixins import BinaryMixin, UnaryMixin


class Instruction(Expression):
    def __init__(self, args: List[Expression]):
        self._args = args

    @abstractmethod
    def evaluate(self, env: Dict[str, Any]) -> Any:
        raise NotImplemented

    def _op(self, *args) -> Any:
        pass

    def __str__(self) -> str:
        args_str = " ".join(str(arg) for arg in self._args)
        op_name = self._get_op_name()
        return f"({op_name} {args_str})"

    def _get_op_name(self) -> str:
        for name, cls in self._dispatch.items():
            if isinstance(self, cls):
                return name
        return "unknown"


############################## Operations ##############################


class Operation(Instruction):
    def evaluate(self, env: Dict[str, Any]) -> Any:
        """
        Evaluate the Operation in a given Expression

        This method processes each argument of the Operation by evaluating them recursively until we meat a variable or a constant.
        It then applies the operation defined in the 'op' method to these evaluated arguments.

        Parameters:
        env (Dict[str, Any]): The environment for variable evaluation.

        Returns:
        Any: The result of the evaluation.
        """
        evaluated_args = [arg.evaluate(env) for arg in self._args]
        return self._op(*evaluated_args)

    def _op(self, *args) -> Any:
        pass


############################## Mathematical Operations ##############################


class Addition(BinaryMixin, Operation):
    def _op(self, a, b):
        return a + b


class Subtraction(BinaryMixin, Operation):
    def _op(self, a, b):
        return a - b


class Multiplication(BinaryMixin, Operation):
    def _op(self, a, b):
        return a * b


class Division(BinaryMixin, Operation):
    def _op(self, a, b):
        if b == 0:
            raise ValueError("Division by zero")
        return a / b


class Power(BinaryMixin, Operation):
    def _op(self, a, b):
        return a**b


class Modulus(BinaryMixin, Operation):
    def _op(self, a, b):
        return a % b


class Reciprocal(UnaryMixin, Operation):
    def _op(self, a):
        if a == 0:
            raise ValueError("Division by zero")
        return 1 / a


class AbsoluteValue(UnaryMixin, Operation):
    def _op(self, a):
        return abs(a)


############################## Comparison Instructions ##############################


class ComparisonOp(BinaryMixin, Operation):
    """
    Base class for comparison Instructions.
    """

    @abstractmethod
    def _op(self, a, b):
        raise NotImplementedError()


class Greater(ComparisonOp):
    def _op(self, a, b):
        return b > a


class GreaterEqual(ComparisonOp):
    def _op(self, a, b):
        return b >= a


class Equal(ComparisonOp):
    def _op(self, a, b):
        return a == b


class NotEqual(ComparisonOp):
    def _op(self, a, b):
        return a != b


class Less(ComparisonOp):
    def _op(self, a, b):
        return b < a


class LessEqual(ComparisonOp):
    def _op(self, a, b):
        return b <= a
