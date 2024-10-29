//
// Created by csimo on 24. 10. 29.
//
#include <iostream>
using namespace std;

class Circle
{
private:
    int radius;
public:
    Circle();
    Circle(int r);
    ~Circle();
    double getArea() {return 3.14*radius*radius;}
    int getRadius() {return radius;}
    void setRadius(int radius){this->radius = radius;}
};

Circle::Circle()
{
    radius = 1;
    cout << "Default constructor called" << radius << endl;
}

Circle::Circle(int radius)
{
    this->radius = radius;
    cout << "Default constructor called" << radius << endl;
}

Circle::~Circle()
{
    cout << "Destructor called" << radius << endl;
}