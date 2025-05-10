    //
// Created by nodove on 24. 12. 5.
//
#include <iostream>
using namespace std;

class Calculator {
public:
    virtual int add(int a, int b) = 0; // 두정수의합리턴
    virtual int subtract(int a, int b) = 0; // 두정수의차리턴
    virtual double average(int a [], int size) = 0; // 배열a의평균리턴. size는배열의크기
};

class GoodCalc
{
public:
    int add (int a, int b) {
        return a + b;
    }

    int subStract (int a, int b) {
        return a - b;
    }

    double average (int a [], int size) {
        double sum = 0;
        for (int i = 0; i < size; i++) sum += a[i];
        return sum / size;
    }
};

int main() {
    int a[] = {1,2,3,4,5};
    Calculator * p = new GoodCalc();
    cout<< p->add(2, 3) << endl;
    cout<< p->subtract(2, 3) << endl;
    cout<< p->average(a, 5) << endl;
    delete p;
}
