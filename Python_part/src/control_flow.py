from typing import Any, Dict

from instruction import Instruction
from utils.mixins import BinaryMixin, QuaternaryMixin


class While(BinaryMixin, Instruction):
    """
    Executes 'expr' repeatedly as long as 'cond' is true.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        cond, expr = self._args  # Extract condition and expression

        while cond.evaluate(env):  # Continuously evaluate condition
            expr.evaluate(env)  # Evaluate expression in the loop


class For(QuaternaryMixin, Instruction):
    """
    Evaluates 'body_expr' multiple times with the loop variable 'i'
    taking values from 'start_expr' to 'end_expr - 1'.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        loop_var, start_expr, end_expr, body_expr = self._args

        start, end = start_expr.evaluate(env), end_expr.evaluate(env)

        loop_variable_name = str(loop_var)

        for i in range(start, end):  # Loop from start to end - 1
            env[loop_variable_name] = i
            body_expr.evaluate(env)
