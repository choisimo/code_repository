
class Circulate:
    def __init__(self):
        self.PI = 3.14

    def circleArea(self, radius):
        return self.PI * radius * radius    # PT * r * r

    def circleCircumference(self, radius):
        return self.PI * radius * 2         # 2 * PI * r


if __name__ == '__main__':
    circulate = Circulate()
    radius = float(input(""))
    circleArea = circulate.circleArea(radius)
    circleCircumference = circulate.circleCircumference(radius)

    print(round(circleArea, 2))          # 원의 면적
    print(round(circleCircumference, 2)) # 원의 둘레