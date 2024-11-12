//
// Created by nodove on 24. 11. 9.
//

/*
 * 1.두 개의 Circle 객체를 교환하는 swap() 함수를 '참조에 의한 호출'이 되도록 작성하고 호출하는 프로그램을 작성하라.
 * */

#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle() : radius(0) {};
    Circle(int r) { this->radius = r; }
    int getRadius() { return radius; }
    void setRadius(int r) { this->radius = r; }
    void swap(Circle &s, Circle &t);
};

void Circle::swap(Circle &s, Circle &t) {
    Circle temp;
    temp = s;
    s = t;
    t = temp;
}

int main() {
    Circle a(5), b(10);
    cout << "Before swapping: a = " << a.getRadius() << ", b = " << b.getRadius() << endl;
    a.swap(a, b);
    cout << "After swapping: a = " << a.getRadius() << ", b = " << b.getRadius() << endl;
    return 0;
}