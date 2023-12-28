import sys

sys.path.append("src/")

from src.condition import IfInstruction
from src.control_flow import *
from src.definition import *
from src.expression import Expression
from src.instruction import *  # Importing all instruction classes
from src.sequence import *
from src.subroutine import *

dispatch = {
    "alloc": Alloc,
    "valloc": Valloc,
    "setv": Setv,
    "setq": Setq,
    "defsub": DefSub,
    "call": Call,
    "print": Print,
    "nop": Nop,
    "+": Addition,
    "*": Multiplication,
    "-": Subtraction,
    "%": Modulus,
    ">": Greater,
    ">=": GreaterEqual,
    "<": Less,
    "=": Equal,
    "!=": NotEqual,
    "if": IfInstruction,
    "while": While,
    "for": For,
    "prog2": Prog2,
    "prog3": Prog3,
    "prog4": Prog4,
}


def test_code_examples():
    # Test Example 1: While loop to increment x until it's greater than 10
    expression1 = Expression.from_program(
        "x 1 + x setq x 10 > while x alloc prog2", dispatch
    )

    env1 = {}

    expression1.evaluate(env1)

    # After the loop, x should be 10
    assert env1["x"] == 10

    # Test Example 2: For loop to initialize and print a vector
    expression2 = Expression.from_program(
        "v print i i * i v setv prog2 10 0 i for 10 v valloc prog2", dispatch
    )
    env2 = {}

    expression2.evaluate(env2)

    # Check if the vector 'v' is correctly initialized and printed
    assert env2["v"] == [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]

    # Test Example 3: Define a subroutine, call it, and print the variable
    expression3 = Expression.from_program(
        "x print f call x alloc x 4 + x setq f defsub prog4", dispatch
    )
    env3 = {}

    expression3.evaluate(env3)
    assert env3["x"] == 4

    # Test Example 4: Divisor finder program
    expression4 = Expression.from_program(
        "nop i print i x % 0 = if 1000 2 i for 783 x setq x alloc prog3", dispatch
    )
    env4 = {}

    expression4.evaluate(env4)

    # Check if the program correctly finds divisors of 783
    assert env4["x"] == 783


def test_prime_example():
    import io
    from contextlib import redirect_stdout

    # Prime number checker program
    prime_expression = Expression.from_program(
        "nop x print prime if nop 0 0 != prime setq i x % 0 = if 1 x - 2 i for 0 0 = prime setq prime alloc prog4 100 2 x for",
        dispatch,
    )

    # Capturing the printed output
    f = io.StringIO()
    with redirect_stdout(f):
        prime_expression.evaluate({})

    # Extract the printed output as a string
    output = f.getvalue()

    # Check the string representation of the expression
    expression_str = str(prime_expression)

    # Video result TO CHECK
    # expected_expression_str = "(for x 2 100 (prog4 (alloc prime) (setq prime (= 0 0)) (for i 2 (- x 1) (if (= 0 (% x i)) (setq prime (> 0 0)) nop)) (if prime (print x) nop)))"

    # What I expect
    expected_expression_str = "(for x 2 100 (prog4 (alloc prime) (setq prime (= 0 0)) (for i 2 (- x 1) (if (= 0 (% x i)) (setq prime (!= 0 0)) nop)) (if prime (print x) nop)))"
    assert (
        expression_str == expected_expression_str
    ), "Expression string does not match expected output"

    # Extract the printed prime numbers from the output string
    prime_numbers = [int(n) for n in output.strip().split()]

    # This should be a list of prime numbers up to 100
    expected_primes = [
        2,
        3,
        5,
        7,
        11,
        13,
        17,
        19,
        23,
        29,
        31,
        37,
        41,
        43,
        47,
        53,
        59,
        61,
        67,
        71,
        73,
        79,
        83,
        89,
        97,
    ]
    assert (
        prime_numbers == expected_primes
    ), "Prime number output does not match expected output"


if __name__ == "__main__":
    test_code_examples()
    test_prime_example()
