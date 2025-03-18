//
// Created by nodove on 24. 11. 26.
//
#include <iostream>
using namespace std;

class Base {
    int a;
protected:
    void setA(int a) {
        this->a = a;
    }
public:
    void showA() {
        cout << a << endl;
    }
};

class Derived : public Base {
    int b;
protected:
    void setB(int b) {
        this->b = b;
    }
public:
    void showB() {
        cout << b << endl;
    }
};

class Derived2 : public Derived {
    int c;
protected:
    void setC(int c) {
        this->c = c;
    }
public:
    void showC() {
        cout << c << endl;
    }
};

int main(){
    Derived2 d2;
    d2.setA(10);
    d2.setB(20);
    d2.setC(30);
    d2.showA();
    d2.showB();
    d2.showC();
}