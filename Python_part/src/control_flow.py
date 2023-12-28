# Jacopo Zacchigna
############################## Control Flow ##############################
from typing import Any, Dict

from instruction import Instruction
from utils.mixins import BinaryMixin, QuaternaryMixin


class ControlFlowOp(Instruction):
    """
    Base class for control flow operations.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        # Pass the arguments as is to the _op method, don't evaluate here
        return self._op(*self._args, env)


class While(BinaryMixin, ControlFlowOp):
    """
    Executes 'expr' repeatedly as long as 'cond' is true.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        # Extract the condition and expression without evaluating
        cond, expr = self._args

        # Continuously re-evaluate the condition within the loop
        while cond.evaluate(env):
            expr.evaluate(env)


class For(QuaternaryMixin, ControlFlowOp):
    """
    Evaluates 'expr' multiple times with the loop variable 'i'
    taking values from 'start' to 'end - 1'.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        # Assuming that the arguments are in the order of loop_variable, start_expression, end_expression, body_expression
        loop_var, start_expr, end_expr, body_expr = self._args

        # Evaluate the start and end values
        start = start_expr.evaluate(env)
        end = end_expr.evaluate(env)
        idx = loop_var.evaluate(env)

        # Loop from start to end - 1
        for i in range(start, end):
            # Update the environment with the current value of the loop variable
            env[idx] = i  # Assuming loop_var has a 'name' attribute
            # Evaluate the body expression
            body_expr.evaluate(env)
