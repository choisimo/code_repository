//
// Created by csimo on 9/19/2024.
//
#include <iostream>
using namespace std;

class Circle{
public:
    int radius;

    Circle();

    Circle(int r);

    double getArea();
};

Circle::Circle() : Circle(radius){}


Circle ::Circle(int r) {
    this->radius = r;
    cout << "������ " << this->radius << endl;
}


double Circle::getArea() {
    return 3.14 * this->radius * this->radius;
}

int main() {
    Circle cir;
    Circle cir2(2);
    cout << "donut ������ " << cir.getArea() << endl;
    cout << "pizza ������ " << cir2.getArea() << endl;

}