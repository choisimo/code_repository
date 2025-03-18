//
// Created by nodove on 24. 12. 3.
//

#ifndef CODE_REPOSITORY_SHAPE_H
#define CODE_REPOSITORY_SHAPE_H

class Shape {
    Shape* next;
protected:
    virtual void draw();
public:
    Shape() { next = NULL; }
    virtual ~Shape() {}
    void paint();
    Shape *add(Shape*p);
    Shape* getNext() { return next;}
};

#endif //CODE_REPOSITORY_SHAPE_H
