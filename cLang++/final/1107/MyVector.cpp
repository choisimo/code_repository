//
// Created by nodove on 24. 11. 7.
//
#include <iostream>
using namespace std;

class MyVector {
    int *ptr;
    int size;
public:
    MyVector(){
        ptr = new int[100];
        size = 100;
    }
    MyVector(int n) {
        ptr = new int[n];
        size = n;
    }
    ~MyVector() {
        delete[] ptr;
    }
};

int main() {
    MyVector *v1, *v2;
    v1 = new MyVector;
    v2 = new MyVector(1024);
    delete v1;
    delete v2;
}