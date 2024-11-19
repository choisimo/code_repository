//
// Created by nodove on 24. 11. 19.
//
#include <iostream>
using namespace std;
#include <string>

class Point {
public:
    int x, y;
public:
    void set(int x, int y);
    void showPoint();
};

void Point::set(int x, int y) {
    this->x = x;
    this->y = y;
}
void Point::showPoint() {
    cout << "(" << x << ", " << y << ")" << endl;
}

class ColorPoint : public Point {
    string color;
public:
    void setColor(string color);
    void showColorPoint();

    bool equals(ColorPoint point);
};

void ColorPoint::setColor(string color) {
    this->color = color;
}

void ColorPoint::showColorPoint() {
    cout << color << ": ";
    showPoint(); // Point 클래스의 showPoint() 호출
}

bool ColorPoint::equals(ColorPoint point) {
    if (x == point.x && y == point.y && color == point.color)
        return true;
    else
        return false;
}

int main(){
    Point p;
    p.set(2, 3);
    p.x = 5; // protected로 선언되어 있어서 접근 불가
    p.y = 5;
    p.showPoint();

    ColorPoint cp; // ColorPoint 객체 생성
    cp.x = 10; // protected로 선언되어 있어서 접근 불가
    cp.y = 10; // protected로 선언되어 있어서 접근 불가
    cp.set(3, 4);
    cp.setColor("Red");
    cp.showColorPoint();

    ColorPoint cp2;
    cp2.set(3, 4);
    cp2.setColor("Red");
    cout << (cp.equals(cp2) ? "같다" : "다르다") << endl;

}