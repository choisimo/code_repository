
class Calculator:
    def __init__(self):
        self.sum = 0
        self.count = 0

    def sum_calculate(self, num):
        self.sum += num
        self.count += 1

    def printAverage(self):
        if self.count != 0:
            print(self.sum / self.count)
        elif self.count == 0:
            print("0 으로 나눌 수 없습니다.")


if __name__ == '__main__':
    calculator = Calculator()
    while True:
        init_input = input("")
        if init_input == "":
            print("빈칸은 입력받을 수 없습니다. 정수형으로 입력해주세요!")
            continue
        try:
            num1 = int(init_input)
            if num1 == 0:
                break
            else:
                calculator.sum_calculate(num1)
                continue
        except TypeError:
            print("정수형 타입만 입력해주세요!")
            continue

    calculator.printAverage()