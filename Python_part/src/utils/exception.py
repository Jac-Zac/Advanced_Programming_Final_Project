# Jacopo Zacchigna .....
class EmptyStackException(Exception):
    """Exception raised when popping from an empty stack."""

    pass


class InvalidExpressionException(Exception):
    """Exception for invalid expressions."""

    pass


class MissingVariableException(Exception):
    """Exception raised when the variable is not present in the env"""

    pass


class UnknownTokenException(Exception):
    """Exception raised when the token is Unknown"""

    pass
