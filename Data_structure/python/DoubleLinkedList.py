from typing import Union

class Node:
    def __init__(self, data: int):
        self.data: int = data
        self.next: Union[Node, None] = None
        self.prev: Union[Node, None] = None


class DoubleLinkedList:

    def __init__(self):
        self.head: Union[Node, None] = None
        self.tail: Union[Node, None] = None

    def append(self, data: int) -> None:
        newNode = Node(data)

        newNode.next = None

        if self.head is None:
            self.head = newNode
            self.tail = newNode
        else:
            newNode.prev = self.tail
            self.tail.next = newNode
            self.tail = newNode


    def remove(self) -> None:
        if self.head is None:
            raise Exception("empty list")

        if self.head == self.tail:
            self.head = None
            self.tail = None
        else:
            self.tail = self.tail.prev
            self.tail.next = None

    def insert(self, data: int, index: int) -> None:
        newNode = Node(data)
        if self.head is None:
            self.head = newNode
            self.tail = self.head
        elif index == 0:
            newNode.next = self.head
            self.head.prev = newNode
            self.head = newNode
        else:
            currentNode = self.head
            currentIndex = 0
            while currentIndex < index - 1:
                currentNode = currentNode.next
                currentIndex += 1
            if currentNode == self.tail:
                if currentIndex < index:
                    raise Exception("index out of boundary")
                else:
                    newNode.prev = self.tail
                    self.tail.next = newNode
                    self.tail = newNode
            else:
                newNode.prev = currentNode
                newNode.next = currentNode.next
                if currentNode.next:
                    currentNode.next.prev = newNode
                currentNode.next = newNode

    def delete(self, index: int) -> None:

        if self.head is None:
            print("list is empty")
            return

        if index < 0:
            raise Exception("index is negative")

        if index == 0:
            if self.head == self.tail:
                self.head = None
                self.tail = None
            else:
                self.head = self.head.next
                self.head.prev = None
            return

        current_node = self.head
        current = 0
        if current_node is None:
            raise Exception("out of range")

        while current < index:
            current_node = current_node.next
            current += 1

        if current_node.next:
            current_node.next.prev = current_node.prev

        if current_node.prev:
            current_node.prev.next = current_node.next

        if current_node == self.tail:
            self.tail = current_node.prev


    def contains(self, data: int) -> bool:
        if self.head is None:
            return False
        else:
            node = self.tail
            while node:
                if node.data == data:
                    return True
                else:
                    node = node.prev
            return False

    def search(self, data: int) -> list:    # return list of indexes
        indexList = []
        current_index = 0
        if self.head is None:
            raise Exception("empty list")
        else:
            node = self.head
            while node:
                if node.data == data:
                    indexList.append(current_index)
                current_index += 1
                node = node.next
            return indexList

    def __getitem__(self, item: int) -> int:
        current_node = self.head
        return current_node.data

    def size(self):
        size = 0
        while self.head is not None:
            size += 1
            self.head = self.head.next
        return size

    def printElements(self) -> None:
        elements = []
        current = self.head
        while current is not None:
            elements.append(current.data)
            current = current.next
        for i in range(len(elements)):
            print(elements[i], end=' ')
        print()

if __name__ == '__main__':
    dll = DoubleLinkedList()
    dll.append(1)
    dll.append(2)
    dll.append(3)
    dll.append(4)
    dll.append(5)
    dll.append(1)
    dll.append(2)
    dll.append(3)
    dll.append(4)
    dll.append(5)
    dll.printElements()         # 1 2 3 4 5 1 2 3 4 5
    dll.remove()
    dll.printElements()         # 1 2 3 4 5 1 2 3 4
    dll.insert(3, 2)
    dll.printElements()         # 1 2 3 3 4 5 1 2 3 4
    dll.delete(2)
    dll.printElements()         # 1 2 3 4 5 1 2 3 4
    print(dll.contains(3))      # True
    print(dll.contains(6))      # False
    print(dll.search(3))        # [2, 7]
    print(dll.search(4))        # [3, 8]
    print(dll.search(6))        # []
    print(dll.search(1))        # [0, 5]
    print(dll.search(5))        # [4, 9]
    print(dll.search(2))        # [1, 6]
    print(dll.search(7))        # []
    print(dll.search(8))        # []

    print(dll[0])               # 1
    print(dll[1])               # 2
    print(dll[2])               # 3
    print(dll[3])               # 4