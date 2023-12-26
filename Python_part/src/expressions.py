from abc import ABC, abstractmethod
from typing import Any, Dict, List

from stack import Stack


class Expression(ABC):
    """
    Abstract base class for expressions.
    """

    def __init__(self):
        if type(self) is Expression:
            raise NotImplementedError("Expression is an abstract class")

    @classmethod
    def from_program(cls, text: str, dispatch: Dict[str, "Operation"]) -> "Expression":
        """
        Parses a program text into an Expression object.

        Args:
            text (str): The program text to parse.
            dispatch (Dict[str, 'Operation']): A mapping from operation symbols to Operation classes.

        Returns:
            Expression: The parsed Expression object.

        Raises:
            ValueError: If an unknown instruction is encountered.
        """
        cls.expresion = text
        cls.dispatch = dispatch
        expression_stack = Stack()

        for token in cls.expresion.split():
            if token in cls.dispatch:
                operator_class = dispatch[token]
                # Make some comment hear KSJDHLhdKLHD
                arguments = [
                    expression_stack.pop() for _ in range(operator_class.arity)
                ]
                expression_stack.push(operator_class(arguments))
            elif token.isdigit():
                expression_stack.push(Constant(int(token)))
            elif token.isalpha():
                expression_stack.push(Variable(token))
            else:
                raise ValueError(f"Unknown istruction: {token}")

        return expression_stack.pop()

    @abstractmethod
    def evaluate(self, env):
        """
        Evaluates the expression.

        Args:
            env (Dict[str, Any]): The environment for variable evaluation.

        Returns:
            Any: The result of the evaluation.
        """
        raise NotImplementedError()

    def __str__(self):
        raise NotImplementedError()


class Variable(Expression):
    def __init__(self, name: str):
        self.name = name

    def evaluate(self, env: Dict[str, Any]) -> Any:
        if self.name not in env:
            raise MissingVariableException(f"Missing variable: {self.name}")

        return env[self.name]

    def __str__(self) -> str:
        return self.name


class Constant(Expression):
    def __init__(self, value):
        self.value = value

    def evaluate(self, env: Dict[str, Any]) -> Any:
        return self.value

    def __str__(self) -> str:
        return f"{self.value}"


class Operation(Expression):
    arity: int = 0

    def __init__(self, args: List[Expression]):
        self.args = args

    def evaluate(self, env: Dict[str, Any]) -> Any:
        evaluated_args = [arg.evaluate(env) for arg in self.args]
        return self.op(*evaluated_args)

    @abstractmethod
    def op(self, *args) -> Any:
        pass

    def __str__(self) -> str:
        args_str = " ".join(str(arg) for arg in self.args)
        op_name = self.get_op_name()
        return f"({op_name} {args_str})"

    def get_op_name(self) -> str:
        for name, cls in self.dispatch.items():
            if cls is self.__class__:
                return name
        return "unknown"


class BinaryOp(Operation):
    arity = 2


class UnaryOp(Operation):
    arity = 1


class Addition(BinaryOp):
    def op(self, a, b):
        return a + b


class Subtraction(BinaryOp):
    def op(self, a, b):
        return a - b


class Multiplication(BinaryOp):
    def op(self, a, b):
        return a * b


class Division(BinaryOp):
    def op(self, a, b):
        if b == 0:
            raise ValueError("Division by zero")
        return a / b


class Power(BinaryOp):
    def op(self, a, b):
        return a**b


class Modulus(BinaryOp):
    def op(self, a, b):
        return a % b


class Reciprocal(UnaryOp):
    def op(self, a):
        if a == 0:
            raise ValueError("Division by zero")
        return 1 / a


class AbsoluteValue(UnaryOp):
    def op(self, a):
        return abs(a)
