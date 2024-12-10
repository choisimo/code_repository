//
// Created by nodove on 24. 12. 10.
//
#include <iostream>
using namespace std;

class Point {
private:
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) {}
    int getX() { return x; }
    int getY() { return y; }
protected:
    void move(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class ColorPoint : protected Point {
private:
    int x, y;
    string color = "BLACK";
public:
    ColorPoint(int x = 0, int y = 0) : Point(x, y) {
        this->x = x;
        this->y = y;
    }
    void setPoint(int x, int y) {
        this->x = x;
        this->y = y;
        move(this->x, this->y);
    }
    void setColor(string color) {
        this->color = color;
    }
    void show()
    {
        cout << color << "색으로 (" << getX() << ", " << getY() << ") 에 위치한 점 입니다." << endl;
    }
};

int main() {
    ColorPoint zeroPoint;
    zeroPoint.show();

    ColorPoint cp(5, 5);
    cp.setPoint(10, 20);
    cp.setColor("BLUE");
    cp.show();
}