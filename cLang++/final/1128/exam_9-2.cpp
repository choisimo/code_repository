//
// Created by nodove on 24. 11. 28.
//
#include <iostream>
using namespace std;

class Base {
    public: void f() {
        cout << "Base::f() called" << endl;
    }
};

class Derived : public Base {
public:
    virtual void f() {
        cout << "Derived::f() called" << endl;
    }
};

int main() {
    Derived d, *pDer = &d;
    pDer->f(); // Derived::f() called

    Base *pBase = pDer;
    pBase = pDer;
    pBase->f(); // Base::f() called
}