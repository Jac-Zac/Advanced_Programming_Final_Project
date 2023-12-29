from typing import Any, Dict

from .instruction import Instruction
from .utils.mixins import BinaryMixin, QuaternaryMixin


class While(BinaryMixin, Instruction):
    """
    Executes 'body' repeatedly as long as 'condition' is true.
    """

    def evaluate(self, env: Dict[str, Any]) -> None:
        condition, body = self._args

        while condition.evaluate(env):
            body.evaluate(env)


class For(QuaternaryMixin, Instruction):
    """
    Executes 'body' multiple times with the loop variable taking values from 'start' to 'end - 1'.
    """

    def evaluate(self, env: Dict[str, Any]) -> None:
        loop_var, start, end, body = self._args

        start_val, end_val = start.evaluate(env), end.evaluate(env)
        loop_var_name = str(loop_var)

        for i in range(start_val, end_val):
            env[loop_var_name] = i
            body.evaluate(env)
