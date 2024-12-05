//
// Created by nodove on 24. 12. 3.
//
#include <iostream>
using namespace std;

class Shape {
public:
    virtual void draw(){
        cout << "--Shape--" << endl;
    }
};

class Circle : public Shape {
public:
    virtual void draw() {
        cout << "--Circle--" << endl;
    }
};

int main() {
    Circle circle;
    Shape *pShape = &circle;

    pShape->draw();
    pShape->Shape::draw();
    return 0;
}