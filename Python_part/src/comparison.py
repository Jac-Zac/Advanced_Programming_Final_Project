# Jacopo Zacchigna
############################## Comparison Instructions ##############################
from abc import abstractmethod

from operation import Operation
from utils.mixins import BinaryMixin


class ComparisonOp(BinaryMixin, Operation):
    """
    Base class for comparison Instructions.
    """

    @abstractmethod
    def _op(self, a, b):
        raise NotImplementedError()


class Greater(ComparisonOp):
    def _op(self, a, b):
        return a > b


class GreaterEqual(ComparisonOp):
    def _op(self, a, b):
        return a >= b


class Equal(ComparisonOp):
    def _op(self, a, b):
        return a == b


class NotEqual(ComparisonOp):
    def _op(self, a, b):
        return a != b


class Less(ComparisonOp):
    def _op(self, a, b):
        return a < b


class LessEqual(ComparisonOp):
    def _op(self, a, b):
        return a <= b
