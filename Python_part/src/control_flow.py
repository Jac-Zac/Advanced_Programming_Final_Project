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
        return self._op(*[arg.evaluate(env) for arg in self._args])


class While(ControlFlowOp):
    """
    Executes 'expr' repeatedly as long as 'cond' is true.
    """

    def _op(self, expr, cond):
        while cond:
            expr.evaluate(self.env)
