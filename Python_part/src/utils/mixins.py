#  In object-oriented programming languages, a mixin (or mix-in)[1][2][3][4] is a class that contains methods for use by other classes without having to be the parent class of those other classes. How those other classes gain access to the mixin's methods depends on the language. Mixins are sometimes described as being "included" rather than "inherited".


# Mixin classes for arity
class NullMixin:
    arity = 1


class UnaryMixin:
    arity = 1


class BinaryMixin:
    arity = 2


class TernaryMixin:
    arity = 3


class QuaternaryMixin:
    arity = 4
