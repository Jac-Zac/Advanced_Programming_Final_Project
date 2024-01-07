# Jacopo Zacchinga SM3201293
from typing import Any, Dict

from .instruction import Instruction
from .utils.mixins import BinaryMixin, TernaryMixin, UnaryMixin


class Alloc(UnaryMixin, Instruction):
    """
    Allocates a single variable and initializes it to 0 in the environment.

    Attributes:
        args (list): Contains one argument, the name of the variable to allocate.
    """

    def evaluate(self, env: Dict[str, Any]) -> None:
        variable_name = str(self._args[0])
        env[variable_name] = 0


class Valloc(BinaryMixin, Instruction):
    """
    Allocates an array for a variable with a specified size and initializes all elements to 0.

    Attributes:
        args (list): Contains two arguments, the name of the variable and the size of the array.
    """

    def evaluate(self, env: Dict[str, Any]) -> None:
        # Expecting a Variable object
        variable_name = str(self._args[0])
        # Evaluate to get the size
        size = self._args[1].evaluate(env)

        if not isinstance(size, int) or size < 0:
            raise ValueError("Array size must be a non-negative integer")

        env[variable_name] = [0] * size


class Setq(BinaryMixin, Instruction):
    """
    Sets the value of a single variable to a new value.

    Attributes:
        args (list): Contains two arguments, the name of the variable and the new value to set.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        variable_name, new_value = str(self._args[0]), self._args[1].evaluate(env)
        env[variable_name] = new_value
        return new_value


class Setv(TernaryMixin, Instruction):
    """
    Sets a specific index of an array variable to a new value.

    Attributes:
        args (list): Contains three arguments, the name of the array variable, the index, and the new value.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        array_name, index, new_value = (
            str(self._args[0]),
            self._args[1].evaluate(env),
            self._args[2].evaluate(env),
        )

        self._validate_array_access(env, array_name, index)

        env[array_name][index] = new_value
        return new_value

    @staticmethod
    def _validate_array_access(env: Dict[str, Any], array_name: str, index: int):
        if not isinstance(env.get(array_name), list):
            raise ValueError(f"Variable {array_name} is not an array")
        if not 0 <= index < len(env[array_name]):
            raise IndexError(f"Index {index} out of bounds for array {array_name}")
