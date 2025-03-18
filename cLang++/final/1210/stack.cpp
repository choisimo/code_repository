//
// Created by nodove on 24. 12. 10.
//
#include <iostream>
using namespace std;

template<typename T>
class MyStack {
private:
    T *arr;
    int top;
    int capacity;
public:
    MyStack(int size = 10) : capacity(size), top(-1) {
        this->arr = new T[capacity];
    }

    ~MyStack() {
        delete[] arr;
    }

    bool isEmpty() {
        return this->top == -1;
    }

    bool isFull() {
        return (top == capacity - 1);
    }

    void push (T data) {
        if (isFull()) {
            cout << "Stack is full" << endl;
            return;
        }

        this->arr[++top] = data;
    }

    T pop() {
        if (isEmpty()) {
            cout << "Stack is empty" << endl;
        }
        this->arr[top].~T();
        T data = this->arr[top--];
        return data;
    }

    T peek() {
        if (isEmpty()) {
            cout << "Stack is empty" << endl;
        }
        return this->arr[this->top];
    }

    int size() {
        return this->top + 1;
    }

    void print() {
        cout << "Stack: " << endl;
        for (int i = 0; i <= this->top; i++) {
            cout << this->arr[i] << " ";
        }
    }
};

class Point {
private:
    int x, y;
public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    void show() {
        cout << x << ", " << y << endl;
    }
};

int main() {
    MyStack<int *> ipStack;
    int *p = new int[3];
    for (int i = 0; i < 3; i++) {
        p[i] = i*10;
    }
    ipStack.push(p);

    int *q = ipStack.pop();
    for (int i = 0; i < 3; i++) {
        cout << q[i] << " ";
    }
    cout << endl;
    delete[] p;

    MyStack<Point> pointStack;
    Point a(2,3), b;
    pointStack.push(a);
    b = pointStack.pop();
    b.show();

    MyStack<Point *> pStack;
    pStack.push(new Point(10, 20));
    Point * pPoint = pStack.pop();
    pPoint->show();

    MyStack<string> strStack;
    string s = "C++";
    strStack.push(s);
    strStack.push("Java");
    cout << strStack.pop() << endl;
    cout << strStack.pop() << endl;
    return 0;
}