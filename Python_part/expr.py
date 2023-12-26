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
    def from_program(cls, text, dispatch):
        cls.expresion = text
        expression_stack = Stack()

        # Do what is needed
        for token in cls.expresion.split():
            if token.isdigit():
                expression_stack.push(Constant(token))
            elif token.isalpha():
                expression_stack.push(Variable(token))
            else:
                expression_stack.push(Operation(token))
                # Get the correct class
                operator_class = dispatch[token]
                # Arguments of to pass
                arguments = [
                    expression_stack.pop() for _ in range(operator_class.arity)
                ]

                expression_stack.push(operator_class(arguments))

        return expression_stack.pop()

    def evaluate(self, env):
        self.env = env
        # raise NotImplementedError()

    def __str__(self):
        return f"The provided Expression is: {self.expresion}"


class MissingVariableException(Exception):
    pass


class Variable(Expression):
    def __init__(self, name):
        self.name = name

    def evaluate(self, env):
        print("I'm hear")
        if self.name not in env:
            raise MissingVariableException

        print(f"Return: {self.name =}")
        return env[self.name]

    def __str__(self):
        return f"{self.name}"


class Constant(Expression):
    def __init__(self, value):
        self.value = value

    def evaluate(self, env):
        return float(self.value)

    def __str__(self):
        return f"{self.value}"


class Operation(Expression):
    def __init__(self, args):
        self.args = args

    def evaluate(self, env):
        pass

    def op(self, *args):
        raise NotImplementedError()

    def __str__(self):
        return f"Operation"
        # raise NotImplementedError()


class BinaryOp(Operation):
    pass


class UnaryOp(Operation):
    pass


class Addition(BinaryOp):
    arity = 2

    def evaluate(self, env):
        return self.args[0].evaluate(env) + self.args[1].evaluate(env)

    def __str__(self):
        return f"({self.args[0]} + {self.args[1]})"


class Subtraction(BinaryOp):
    pass


class Division(BinaryOp):
    pass


class Multiplication(BinaryOp):
    arity = 2

    def evaluate(self, env):
        return self.args[0].evaluate(env) * self.args[1].evaluate(env)

    def __str__(self):
        return f"({self.args[0]} * {self.args[1]})"


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
example = "2 x + 3 *"

e = Expression.from_program(example, d)

print(e)

res = e.evaluate({"x": 3})

# res = e.evaluate({"x": 3, "y": 7})
print(res)

# Ouput attest:
# (1/ (+ (1/ y) (** 2 (abs (/ (- 5 6) (* x (+ 3 2)))))))
# 0.84022932953024
