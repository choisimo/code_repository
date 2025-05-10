//
// Created by nodove on 24. 11. 28.
//
#include <iostream>
using namespace std;

class Base {
public:
    void f() {
        cout << "Base::f() called" << endl;
    }
};


class Derived : public Base {
public:
    void f() {
        cout << "Derived::f() called" << endl;
    }
};


class GrandDerived : public Derived {
public:
    void f() {
        cout << "GrandDerived::f() called" << endl;
    }
};


int main() {
    GrandDerived g;                             // Derived dg;
    Base *bp;                                   // bp=dp=gp=&dg;
    Derived *dp;
    GrandDerived *gp;
    bp = dp = gp = &g;

    bp->f(); // Base::f() called
    dp->f(); // Derived::f() called
    gp->f(); // GrandDerived::f() called

    return 0;
}