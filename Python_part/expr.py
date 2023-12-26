from typing import Dict, List


class EmptyStackException(Exception):
    pass


class Stack:
    def __init__(self):
        self.data = []

    def push(self, x):
        self.data.append(x)

    def pop(self):
        if self.data == []:
            raise EmptyStackException
        res = self.data[-1]
        self.data = self.data[0:-1]
        return res

    def __str__(self):
        return " ".join([str(s) for s in self.data])


class Expression:
    def __init__(self):
        raise NotImplementedError()

    @classmethod
    def from_program(cls, text: str, dispatch: Dict[str, type]):
        cls.expresion = text
        cls.dispatch = dispatch
        expression_stack = Stack()

        # Do what is needed
        for token in cls.expresion.split():
            if token in cls.dispatch:
                # Get the correct class
                operator_class = dispatch[token]
                # Arguments of to pass
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

    def evaluate(self, env):
        raise NotImplementedError()

    # def __str__(self):
    #     return f"The provided Expression is: {self.expresion}"


class MissingVariableException(Exception):
    pass


class Variable(Expression):
    def __init__(self, name: str):
        self.name = name

    def evaluate(self, env):
        if self.name not in env:
            raise MissingVariableException(f"Missing variable: {self.name}")

        return env[self.name]

    def __str__(self):
        return f"{self.name}"


class Constant(Expression):
    def __init__(self, value):
        self.value = value

    def evaluate(self, env):
        return self.value

    def __str__(self):
        return f"{self.value}"


class Operation(Expression):
    def __init__(self, args):
        self.args = args

    def evaluate(self, env):
        # Look at how it is working again
        return self.op(*[arg.evaluate(env) for arg in self.args])

    def op(self, *args):
        raise NotImplementedError()

    def __str__(self):
        args_str = " ".join(str(arg) for arg in self.args)
        return f"{self.__class__.__name__}({args_str})"


class BinaryOp(Operation):
    arity = 2
    # def evaluate(self, env):
    #     self.left = self.args[0].evaluate(env)
    #     self.right = self.args[1].evaluate(env)
    #     return super().evaluate(env)


class UnaryOp(Operation):
    arity = 1


class Addition(BinaryOp):
    # def op(self, *args):
    #     return sum(args)
    def op(self, a, b):
        return a + b

    def __str__(self):
        return f"({str(self.args[0])} + {str(self.args[1])})"


class Subtraction(BinaryOp):
    pass


class Division(BinaryOp):
    pass


class Multiplication(BinaryOp):
    def op(self, a, b):
        return a * b

    def __str__(self):
        return f"({str(self.args[0])} * {str(self.args[1])})"


class Power(BinaryOp):
    pass


class Modulus(BinaryOp):
    pass


class Reciprocal(UnaryOp):
    pass


class AbsoluteValue(UnaryOp):
    pass


d = {
    "+": Addition,
    "*": Multiplication,
    "**": Power,
    "-": Subtraction,
    "/": Division,
    "1/": Reciprocal,
    "abs": AbsoluteValue,
}

# example = "2 3 + x * 6 5 - / abs 2 ** y 1/ + 1/"
# example = "2 x +"
example = "2 x + 3 *"

e = Expression.from_program(example, d)

print(e)

res = e.evaluate({"x": 3})

# res = e.evaluate({"x": 3, "y": 7})
# print(res)

# Output attest:
# (1/ (+ (1/ y) (** 2 (abs (/ (- 5 6) (* x (+ 3 2)))))))
# 0.84022932953024

# YOU can add a method compile to compile into C code that can then be runned
