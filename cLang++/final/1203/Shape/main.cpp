//
// Created by nodove on 24. 12. 3.
//
#include <iostream>
#include "Shape.h"
#include "Line.h"
#include "Rect.h"
#include "Circle.h"
#include "Triangle.h"
using namespace std;

int main() {
    Shape *pStart = NULL;
    Shape *pLast;

    pStart = new Line();
    pLast = pStart;

    pLast = pLast->add(new Triangle());
    pLast = pLast->add(new Rect());
    pLast = pLast->add(new Circle());
    pLast = pLast->add(new Triangle());

    Shape*p = pStart;
    while (p!=NULL) {
        p->paint();
        p = p->getNext();
    }

    p = pStart;
    while (p != NULL) {
        Shape *q = p->getNext();
        delete p;
        p = q;
    }

    return 0;
}