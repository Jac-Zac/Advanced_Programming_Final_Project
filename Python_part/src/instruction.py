# Jacopo Zacchigna
from abc import abstractmethod
from typing import Any, Dict, List

from expression import Expression


class Instruction(Expression):
    def __init__(self, args: List[Expression]):
        self._args = args

    @abstractmethod
    def evaluate(self, env: Dict[str, Any]) -> Any:
        raise NotImplemented

    def _op(self, *args) -> Any:
        pass

    def __str__(self) -> str:
        args_str = " ".join(str(arg) for arg in self._args)
        op_name = self._get_op_name()
        return f"({op_name} {args_str})"

    def _get_op_name(self) -> str:
        for name, cls in Expression._dispatch.items():
            if isinstance(self, cls):
                return name
        return "unknown"
