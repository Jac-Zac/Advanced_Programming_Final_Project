# Jacopo Zacchigna
############################## Control Flow ##############################
from typing import Any, Dict

from instruction import Instruction
from utils.mixins import BinaryMixin


class ControlFlowOp(BinaryMixin, Instruction):
    """
    Base class for control flow operations.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        # Pass the arguments as is to the _op method, don't evaluate here
        return self._op(*self._args, env)


class While(ControlFlowOp):
    """
    Executes 'expr' repeatedly as long as 'cond' is true.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        # Extract the condition and expression without evaluating
        cond, expr = self._args

        # Continuously re-evaluate the condition within the loop
        while cond.evaluate(env):
            expr.evaluate(env)
