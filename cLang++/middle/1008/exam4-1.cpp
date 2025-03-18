//
// Created by csimo on 10/8/2024.
//

#include <iostream>
using namespace std;

class Circle{
private:
    int radius;
public:
    Circle() {
        this->radius = 1;
    }
    Circle(int r) {
        radius = r;
    }

    void setRadiusWithPointer(Circle *ptr){
        (*ptr).radius = 1;
        ptr->radius = 1;
    }

    double getArea();
};

double Circle::getArea(){
    return 3.14 * radius * radius;
}
