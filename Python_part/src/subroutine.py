# Jacopo Zacchinga SM3201293
from typing import Any, Dict

from .instruction import Instruction
from .utils.mixins import BinaryMixin, UnaryMixin


class DefSub(Instruction, BinaryMixin):
    """
    Defines a subroutine and associates it with a variable name.
    """

    def evaluate(self, env: Dict[str, Any]) -> None:
        variable_name, subroutine_body = str(self._args[0]), self._args[1]

        # Depends if we want to allow duplicate function names let's avoid it
        if variable_name in env:
            raise ValueError(
                f"Symbol: {variable_name} already exists in the environment"
            )

        # Store the subroutine as a callable function in the environment
        env[variable_name] = lambda: subroutine_body.evaluate(env)


class Call(Instruction, UnaryMixin):
    """
    Calls a subroutine associated with a given variable name.
    """

    def evaluate(self, env: Dict[str, Any]):
        # Get the name
        function_name = str(self._args[0])

        if function_name not in env:
            raise ValueError(
                f"Symbol: {function_name} is not defined in the environment"
            )

        # Call the stored subroutine
        subroutine = env[function_name]
        return subroutine()
