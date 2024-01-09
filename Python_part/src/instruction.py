# Jacopo Zacchinga SM3201293
import time
from abc import abstractmethod
from typing import Any, Dict, List

from .expression import Expression
from .utils.mixins import BinaryMixin, NullMixin, UnaryMixin


class Instruction(Expression):
    """
    Abstract Class for a general instruction
    """

    def __init__(self, args: List[Expression]):
        self._args = args

    @abstractmethod
    def evaluate(self, env: Dict[str, Any]) -> Any:
        raise NotImplementedError()

    def _op(self, *args):
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


class Nop(NullMixin, Instruction):
    """
    Does not perform any operation.
    """

    def evaluate(self, env: Dict[str, Any]) -> None:
        # This operation does nothing
        pass

    def __str__(self) -> str:
        return f"nop"


# I have added this since I thought I might use it in the future
class Sleep(UnaryMixin, Instruction):
    """
    Sleep for n seconds based on the evaluation of the previous argument
    """

    def evaluate(self, env: Dict[str, Any]) -> None:
        # Sleep for the amount that is obtained by the evaluation of the expr
        time.sleep(self._args[0].evaluate(env))


class Print(UnaryMixin, Instruction):
    """
    Evaluates an expression and prints the result.
    """

    def evaluate(self, env: Dict[str, Any]) -> Expression:
        expression = self._args[0]

        # Evaluate the expression and print its result
        result = expression.evaluate(env)
        print(result)
        return result


############################## Operations ##############################


class Operation(Instruction):
    """
    Specific type of instructions that involve arithmetical logic
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        """
        Evaluate all of the args and then apply the operation to all of them
        """
        evaluated_args = [arg.evaluate(env) for arg in self._args]
        return self._op(*evaluated_args)

    @abstractmethod
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


class Greater(ComparisonOp):
    def _op(self, a, b) -> bool:
        return a > b


class GreaterEqual(ComparisonOp):
    def _op(self, a, b) -> bool:
        return a >= b


class Equal(ComparisonOp):
    def _op(self, a, b) -> bool:
        return a == b


class NotEqual(ComparisonOp):
    def _op(self, a, b) -> bool:
        return a != b


class Less(ComparisonOp):
    def _op(self, a, b) -> bool:
        return a < b


class LessEqual(ComparisonOp):
    def _op(self, a, b) -> bool:
        return a <= b
