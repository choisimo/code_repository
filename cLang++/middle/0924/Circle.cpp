//
// Created by csimo on 9/24/2024.
//
#include <iostream>
using namespace std;

class Circle {
public:
    int radius;

    Circle();

    Circle(int i);

    double getArea();

};

Circle::Circle() : Circle(1){} // 위임 생성자

Circle::Circle(int r){
    this->radius = r;
    cout << "반지름" << radius << "원 생성" << endl;
}

double Circle::getArea(){
    return 3.14 * this->radius * this->radius;
}

int main(){
    Circle donut;
    double area = donut.getArea();
    cout << "donut 면적은" << area << endl;

    Circle pizza(30);
    double area2 = pizza.getArea();
    cout << "pizza 면적은" << area2 << endl;

    Circle *headCircle = new Circle();
    cout << headCircle->getArea() << endl;
    delete headCircle;
}