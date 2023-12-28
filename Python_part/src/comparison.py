# Jacopo Zacchigna
############################## Comparison Instructions ##############################
from operation import BinaryOp


class ComparisonOp(BinaryOp):
    """
    Base class for comparison Instructions.
    """


class Greater(ComparisonOp):
    def op(self, a, b):
        return a > b


class GreaterEqual(ComparisonOp):
    def op(self, a, b):
        return a >= b


class Equal(ComparisonOp):
    def op(self, a, b):
        return a == b


class NotEqual(ComparisonOp):
    def op(self, a, b):
        return a != b


class Less(ComparisonOp):
    def op(self, a, b):
        return a < b


class LessEqual(ComparisonOp):
    def op(self, a, b):
        return a <= b
