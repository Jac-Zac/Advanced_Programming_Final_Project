# Jacopo Zacchinga SM3201293
from typing import Any, Dict

from .instruction import Instruction
from .utils.mixins import BinaryMixin, QuaternaryMixin


class While(BinaryMixin, Instruction):
    """
    Executes 'body' repeatedly as long as 'condition' is true.
    """

    def evaluate(self, env: Dict[str, Any]) -> None:
        # Get stopping condition and body of the loop
        condition, body = self._args

        while condition.evaluate(env):
            body.evaluate(env)


class For(QuaternaryMixin, Instruction):
    """
    Executes 'body' multiple times with the loop variable taking values from 'start' to 'end - 1'.
    """

    def evaluate(self, env: Dict[str, Any]) -> None:
        # Get the args in the correct order
        loop_var, start, end, body = self._args

        # Evaluate the start and end value
        start_val, end_val = start.evaluate(env), end.evaluate(env)
        # Get the name for the index of the loop
        loop_var_name = str(loop_var)

        for i in range(start_val, end_val):
            # Update the index value
            env[loop_var_name] = i
            body.evaluate(env)
