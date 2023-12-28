from typing import Any, Dict

from .instruction import Instruction
from .utils.mixins import (BinaryMixin, QuaternaryMixin, TernaryMixin,
                           UnaryMixin)


class ProgN(Instruction):
    """
    Executes a sequence of operations in reverse order (due to stack behavior in Polish notation)
    and returns the value of what was originally the first operation.

    Attributes:
        args (list): A list of expressions to be evaluated in reverse order.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        if not self._args:
            raise ValueError("ProgN requires at least one argument")

        # Evaluate all expressions in reverse order
        for arg in self._args[:-1]:
            arg.evaluate(env)

        # Evaluate and return the result of the original first expression
        return self._args[-1].evaluate(env)


class Prog1(ProgN, UnaryMixin):
    pass


class Prog2(ProgN, BinaryMixin):
    pass


class Prog3(ProgN, TernaryMixin):
    pass


class Prog4(ProgN, QuaternaryMixin):
    pass
