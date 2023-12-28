from typing import Any, Dict

from .instruction import Instruction
from .utils.mixins import TernaryMixin


class IfInstruction(TernaryMixin, Instruction):
    """
    Executes a conditional operation. Evaluates 'cond' and based on its truthiness,
    evaluates either 'if_yes' or 'if_no', returning the result of the evaluated branch.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        cond, if_yes, if_no = self._args
        return if_yes.evaluate(env) if cond.evaluate(env) else if_no.evaluate(env)
