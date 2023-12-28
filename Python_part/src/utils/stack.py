# Jacopo Zacchigna .....
from typing import Any

from utils.exception import *


class Stack:
    def __init__(self):
        self.data = []

    def push(self, x: Any) -> None:
        self.data.append(x)

    # def pop(self):
    #     if self.data == []:
    #         raise EmptyStackException
    #     res = self.data[-1]
    #     self.data = self.data[0:-1]
    #     return res

    # Cleaner implementation
    def pop(self) -> Any:
        if not self.data:
            raise EmptyStackException
        return self.data.pop()

    # def __str__(self):
    #     return " ".join([str(s) for s in self.data])

    # Cleaner implementation with map
    def __str__(self) -> str:
        return " ".join(map(str, self.data))
