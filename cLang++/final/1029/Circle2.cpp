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
    Circle() {this->radius = 1;};
    Circle(int radius)
    {
        this->radius = radius;
    }
    double getArea()
    {
        return this->radius * this->radius * 3.14;
    }
};

Circle getCircle(int radius)
{

}
