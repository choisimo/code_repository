//
// Created by nodove on 24. 11. 9.
//
/*
 * 다음과 같이 선언돤 정수를 저장하는 스택 클래스 MyIntStack을 구현하라.
 * MyIntStack 스택에 저장훌 수 있는 정수의 최대 개수는 10개이다.
 * -> MyIntStack 클래스를 활용하는 코드와 실행 결과는 다음과 같다.
 * */

#define array_size 10
#include <iostream>
using namespace std;

class MyIntStack {
private:
    int array[array_size] = {0};
    int size = array_size;  // size of array is 10 elements
    int top = 0;            // top default value is 0
public:
    MyIntStack() {};

    bool push(int n);

    bool pop(int &n);

    bool isFull() const {
        if (top == size) {
            return true;
        }
        return false;
    }
    bool isEmpty() const {
        if (top <= 0) {
            return true;
        }
        return false;
    }
};

bool MyIntStack::push(int n) {
    if (top == size) {
        cout << "stack is full" << endl;
        return false;
    } else {
        this->array[top++] = n;             // push and top++7
        return true;
    }
}

bool MyIntStack::pop(int &n) {
    if (top <= 0) {
        cout << "stack is empty" << endl;
        return false;
    } else {
        n = this->array[--top];                     // top-- and pop -> top = n + 1 element
        return true;
    }
}

int main() {
    MyIntStack stack;
    for (int i = 0; i < 11; i++) {
        if (!stack.isFull()) {
            cout << i << " ";
            stack.push(i);
        } else {
            cout << i + 1 << "번 째 stack full" << endl;
        }
    }

    int n;
    for (int i = 0; i < 11; i++) {
        if (!stack.isEmpty()) {
            stack.pop(n);
            cout << n << " ";
        } else {
            cout << i + 1 << "번 째 stack empty" << endl;
        }
    }
}