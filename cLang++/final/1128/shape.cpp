//
// Created by nodove on 24. 11. 28.
//
#include <iostream>
using namespace std;

class Shape {
public:
    void paint() {
        draw();
    }

    virtual void draw(){
        cout << "Shape::draw() called" << endl;
    }
};

int main() {
    Shape *pShape = new Shape();
    pShape->paint();
    delete pShape;
}


