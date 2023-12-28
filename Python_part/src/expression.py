# Jacopo Zacchigna ...
from abc import ABC, abstractmethod
from typing import Any, Dict

from utils.exception import *
from utils.stack import Stack


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
        cls._dispatch = dispatch
        expression_stack = Stack()

        for token in text.split():
            if token in cls._dispatch:
                operation_class = dispatch[token]
                # Make some comment hear KSJDHLhdKLHD
                if token == "nop":
                    expression_stack.push(operation_class(None))
                else:
                    arguments = [
                        expression_stack.pop() for _ in range(operation_class.arity)
                    ]
                    expression_stack.push(operation_class(arguments))

            elif token.isdigit():
                expression_stack.push(Constant(int(token)))
            elif token.isalpha():
                expression_stack.push(Variable(token))
            else:
                raise ValueError(f"Unknown istruction: {token}")

        return expression_stack.pop()

    @abstractmethod
    def evaluate(self, env):
        raise NotImplementedError()

    def __str__(self):
        raise NotImplementedError()


############################## Base definition ##############################


class Variable(Expression):
    def __init__(self, name: str):
        self._name = name

    def evaluate(self, env: Dict[str, Any]) -> Any:
        if self._name not in env:
            raise MissingVariableException(f"Missing variable: {self._name}")

        return env[self._name]

    def __str__(self) -> str:
        return self._name


class Constant(Expression):
    def __init__(self, value):
        self._value = value  # Protected member

    def evaluate(self, env: Dict[str, Any]) -> Any:
        return self._value

    def __str__(self) -> str:
        return f"{self._value}"
