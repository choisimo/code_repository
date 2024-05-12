class Queue:

    def __init__(self): # 생성자
        self.array: list = []

    def enqueue(self, item):
        self.array.append(item)

    def dequeue(self):
        if self.isEmpty():
            raise Exception("Queue is empty")
        return self.array.pop(0)

    def peak(self):
        if self.isEmpty():
            raise Exception("Queue is empty")
        return self.array[0]
    def printElements(self):
        for i in range(len(self.array)):
            print(self.array[i], end=' ')
        print()

    def isEmpty(self):
        return len(self.array) == 0
    def size(self):
        return len(self.array)



if __name__ == '__main__':

    queue = Queue()

    queue.enqueue(1)
    queue.enqueue(2)
    queue.enqueue(3)
    queue.enqueue(4)
    queue.enqueue(5)

    queue.printElements()       # 1 2 3 4 5

    print(queue.isEmpty())      # False

    print(queue.peak())         # 1
    print(queue.dequeue())      # 1
    print(queue.dequeue())      # 2
    print(queue.dequeue())      # 3
    print(queue.peak())         # 4
    print(queue.dequeue())      # 4
    print(queue.dequeue())      # 5

    print(queue.isEmpty())      # True




