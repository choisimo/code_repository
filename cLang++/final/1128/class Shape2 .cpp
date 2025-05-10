//
// Created by nodove on 24. 11. 28.
//
#include <iostream>
using namespace std;

class Shape2 {
public:
    void paint() {
        draw();
    }

    virtual void draw() {
        cout << "Shape2::draw() called" << endl;
    }
};


class Circle : public Shape2 {
public:
    virtual void draw() {
        cout << "Circle::draw() called" << endl;
    }
};

int main() {
    Shape2 *pShape = new Circle(); // up casting
    pShape->paint();
    delete pShape;
}