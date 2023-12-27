from typing import Any, Dict

from operation import BinaryOp, Operation, TernaryOp, UnaryOp


class ProgN(Operation):
    """
    Executes a sequence of operations and returns the value of the first operation.

    Attributes:
        arcs (list): A list of expressions to be evaluated.
    """

    def evaluate(self, env: Dict[str, Any]) -> Any:
        if not self.args:
            raise ValueError("ProgN requires at least one argument")

        results = [arg.evaluate(env) for arg in self.args]
        return results[0] if results else None


class Prog2(ProgN, BinaryOp):
    pass
