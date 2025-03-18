//
// Created by nodove on 24. 12. 5.
//
#include <iostream>
using namespace std;

class Circle {
    int radius;
public:
    Circle(int r = 1) : radius(r) {}
    void show() { cout << "반지름이 " << radius << "인 원" << endl; }
    int getRadius() { return radius; }
};


template <class T>
void mySwap(T & a, T & b) {
 T tmp;
 tmp = a;
 a =  b;
 b = tmp;
}

int main() {
    int a = 4, b = 5;
    mySwap(a, b);
    cout << a << ' ' << b << endl;

    double c = 0.3, d = 12.5;
    mySwap(c, d);
    cout << c << ' ' << d << endl;

    Circle donut(5), pizza(20);
    mySwap(donut, pizza);
    cout << "donut의 반지름 = " << donut.getRadius() << endl;
    cout << "pizza의 반지름 = " << pizza.getRadius() << endl;
}