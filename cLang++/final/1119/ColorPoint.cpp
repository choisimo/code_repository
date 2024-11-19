//
// Created by nodove on 24. 11. 19.
//
#include <iostream>
#include <cstring>
using namespace std;

class Point {
private:
    int x, y;
public:
    void set(int x, int y) {
        this->x = x;
        this->y = y;
    }
    void showPoint() {
        cout << "(" << x << ", " << y << ")" << endl;
    }
};

class ColorPoint : public Point {
private:
    string color;
public:
    void setColor(string color) {
        this->color = color;
    }
    void showColorPoint() {
        cout << color << ": ";
        showPoint();                        // Point 클래스의 showPoint() 호출
    }
};

int main() {
    Point p;
    p.set(2, 3);
    p.showPoint();

    ColorPoint cp;
    cp.set(3, 4);
    cp.setColor("Red");
    cp.showColorPoint();
}