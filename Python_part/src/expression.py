# Jacopo Zacchinga SM3201293
from abc import ABC, abstractmethod
from typing import Any, Dict, List

from .utils.exception import *
from .utils.stack import Stack


class Expression(ABC):
    """
    Abstract base class for expressions.
    """

    # Since there is only on dispatch it is more efficient
    __slots__ = ["_dispatch"]

    def __init__(self):
        if type(self) is Expression:
            raise NotImplementedError("Expression is an abstract class")

    @classmethod
    def from_program(cls, text: str, dispatch: Dict[str, "Operation"]) -> "Expression":
        """
        Parses a program text into an Expression object.
        """
        cls.__validate_dispatch(dispatch)
        cls._dispatch = dispatch  # Set the dispatch table for this instance
        tokens = text.split()

        # Pop from the stack the operation
        return cls.__process_tokens(tokens, dispatch).pop()

    @staticmethod
    def __validate_dispatch(dispatch):
        if not dispatch:
            raise ValueError("Dispatch table cannot be empty")

    @staticmethod
    def __process_tokens(tokens: List[str], dispatch) -> Stack:
        expression_stack = Stack()
        for token in tokens:
            if token in dispatch:
                operation_class = dispatch[token]
                # Pop their arguments from the stack based on the arity of the operation
                arguments = [
                    expression_stack.pop() for _ in range(operation_class.arity)
                ]
                # Push the operation to the stack
                expression_stack.push(operation_class(arguments))

            # If a variable or a constant deal with it separately
            elif token == "true" or token == "false":
                expression_stack.push(Bool(token))
            elif token.isalpha():
                expression_stack.push(Variable(token))
            elif token.isdigit():
                expression_stack.push(Constant(int(token)))
            else:
                raise UnknownTokenException(f"Unknown instruction: {token}")

        return expression_stack

    @abstractmethod
    def evaluate(self, env: Dict[str, Any]):
        raise NotImplementedError()

    def __str__(self) -> str:
        raise NotImplementedError()


############################## Base definition ##############################


class Variable(Expression):
    """
    Single variable with corresponding value (one of the base expressions)
    """

    def __init__(self, name: str):
        self._name = name

    def evaluate(self, env: Dict[str, Any]) -> int:
        if self._name not in env:
            raise MissingVariableException(f"Missing variable: {self._name}")

        # If we only wanted int we could do:
        # return int(env[self._name])

        return env[self._name]

    def __str__(self) -> str:
        return self._name


class Constant(Expression):
    """
    Single constant (one of the base expressions)
    """

    def __init__(self, value):
        self._value = value

    def evaluate(self, env: Dict[str, Any]) -> int:
        return self._value

    def __str__(self) -> str:
        return f"{self._value}"


class Bool(Expression):
    """
    Single Bool (one of the base expressions)
    """

    def __init__(self, value):
        self._value = str(value)

    def evaluate(self, env: Dict[str, Any]) -> bool:
        return True if self._value == "true" else False

    def __str__(self) -> str:
        return f"{self._value}"
