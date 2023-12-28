# Jacopo Zacchigna
from abc import abstractmethod
from typing import Any, Dict

from expression import Expression
from instruction import Instruction
from utils.mixins import BinaryMixin, UnaryMixin


class Operation(Instruction):
    def evaluate(self, env: Dict[str, Any]) -> Any:
        pass
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
