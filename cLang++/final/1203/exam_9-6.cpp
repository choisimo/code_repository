//
// Created by nodove on 24. 12. 3.
//
#include <iostream>
using namespace std;

class Base {
public:
    virtual ~Base() {
        cout << "~Base()" << endl;
    }
};


class Derived : public Base {
    virtaul ~Derived() {
        cout << "~Derived()" << endl;
    }
};

int main () {
    Base *dp = new Derived();
    Base *bp = new Derived();

    delete dp;
    delete bp;
}