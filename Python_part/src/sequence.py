# Jacopo Zacchinga SM3201293
from typing import Any, Dict

from .instruction import Instruction
from .utils.mixins import (BinaryMixin, QuaternaryMixin, TernaryMixin,
                           UnaryMixin)


class ProgN(Instruction):
    """
    Base class for prog of N operations
    Executes a sequence of operations in reverse order and returns the value of what was originally the first operation.
    """

    def evaluate(self, env: Dict[str, Any]):
        # Prog0 wouldn't really make much sense
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
