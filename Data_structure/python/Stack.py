# Stack class 구현

class Stack:

    def __init__(self): # 생성자
        self.array: list = []

    def push(self, item: int) -> None:  # null / nullptr
        self.array.append(item)
    def pop(self) -> int:
        if self.isEmpty():
            raise Exception("Stack is empty")
        return self.array.pop()

    def peak(self) -> int:
        if self.isEmpty():
            raise Exception("Stack is empty")
        return self.array[-1]

    def printElements(self) -> None:
        for i in range(len(self.array)):
            print(self.array[i], end=' ')
        print()


    def isEmpty(self) -> bool:
        return len(self.array) == 0

    def size(self):
        return len(self.array)


if __name__ == '__main__':

    stack = Stack()

    stack.push(1)
    stack.push(2)
    stack.push(3)
    stack.push(4)
    stack.push(5)

    stack.printElements()       # 1 2 3 4 5

    print(stack.isEmpty())      # False

    print(stack.peak())         # 5
    print(stack.pop())          # 5
    print(stack.pop())          # 4
    print(stack.pop())          # 3
    print(stack.peak())         # 2
    print(stack.pop())          # 2
    print(stack.pop())          # 1

    print(stack.isEmpty())      # True

