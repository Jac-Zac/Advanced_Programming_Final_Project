from typing import Any, Dict

from instruction import Instruction
from utils.mixins import BinaryMixin, TernaryMixin, UnaryMixin


class Alloc(UnaryMixin, Instruction):
    """
    Allocates a single variable and initializes it to 0 in the environment.

    Attributes:
        args (list): Contains one argument, the name of the variable to allocate.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        variable_name = str(self._args[0])
        if variable_name in env:
            raise ValueError(f"Variable '{variable_name}' already allocated")

        env[variable_name] = 0
        return None


class Valloc(BinaryMixin, Instruction):
    """
    Allocates an array for a variable with a specified size and initializes all elements to 0.

    Attributes:
        args (list): Contains two arguments, the name of the variable and the size of the array.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        # Expecting a Variable object
        variable_name = str(self._args[0])
        # Evaluate to get the size
        size = self._args[1].evaluate(env)

        if not isinstance(size, int) or size < 0:
            raise ValueError("Array size must be a non-negative integer")

        env[variable_name] = [0] * size
        return None


class Setq(BinaryMixin, Instruction):
    """
    Sets the value of a single variable to a new value.

    Attributes:
        args (list): Contains two arguments, the name of the variable and the new value to set.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        variable_name = str(self._args[0])
        new_value = self._args[1].evaluate(env)
        env[variable_name] = new_value
        return new_value


class Setv(TernaryMixin, Instruction):
    """
    Sets a specific index of an array variable to a new value.

    Attributes:
        args (list): Contains three arguments, the name of the array variable, the index, and the new value.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        array_name = str(self._args[0])  # String that represent the variable name
        new_value = self._args[2].evaluate(env)  # Evaluate to get the new value
        index = self._args[1].evaluate(env)  # Evaluate to get the index

        if not isinstance(env[array_name], list):
            raise ValueError(f"Variable {array_name} is not an array")
        if not 0 <= index < len(env[array_name]):
            raise IndexError(f"Index {index} out of bounds for array {array_name}")

        env[array_name][index] = new_value
        return new_value
