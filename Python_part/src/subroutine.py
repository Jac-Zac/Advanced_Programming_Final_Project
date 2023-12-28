from typing import Any, Dict

from instruction import Instruction
from utils.mixins import BinaryMixin, UnaryMixin


class DefSub(Instruction, BinaryMixin):
    """
    Defines a subroutine and associates it with a variable.
    """

    def evaluate(self, env: Dict[str, Any]) -> None:
        variable_name = str(self._args[0])
        expression = self._args[1]

        if variable_name in env:
            raise ValueError(f"Symbol: {variable_name} already exists")

        # Store the expression as a callable function in the environment
        env[variable_name] = lambda: expression.evaluate(env)


class Call(Instruction, UnaryMixin):
    """
    Calls a subroutine associated with a variable.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        variable_name = str(self._args[0])

        if variable_name not in env:
            raise ValueError(f"Symbol: {variable_name} is not defined")

        # Call the stored function
        subroutine = env[variable_name]
        return subroutine()
