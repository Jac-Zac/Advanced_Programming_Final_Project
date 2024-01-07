# Jacopo Zacchinga SM3201293
from typing import Any, Dict

from .expression import Expression
from .instruction import Instruction
from .utils.mixins import TernaryMixin


class If(TernaryMixin, Instruction):
    """
    Executes a conditional operation. Evaluates 'cond' and based on its truthiness,
    evaluates either 'if_yes' or 'if_no', returning the result of the evaluated branch.
    """

    def evaluate(self, env: Dict[str, Any]) -> Expression:
        condition, true_branch, false_branch = self._args
        return (
            true_branch.evaluate(env)
            if condition.evaluate(env)
            else false_branch.evaluate(env)
        )
