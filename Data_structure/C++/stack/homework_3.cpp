//
// Created by csimo on 3/29/2024.
//
#include <iostream>
#include <stdexcept>
using namespace std;
class Stack {
    char* array;
    int capacity;
    int top;

public:
    Stack() : capacity(10), top(-1) {
        this->array = new char[this->capacity];
    }

    ~Stack() {
        delete[] this->array; // 동적 할당된 메모리 해제
    }

    void push(char item) {
        if (this->isFull()) {
            throw std::out_of_range("Stack overflow");
        }
        this->array[++this->top] = item;
    }

    char pop() {
        if (this->isEmpty()) {
            throw std::out_of_range("Stack underflow");
        }
        return this->array[this->top--];
    }

    bool isEmpty() const {
        return this->top == -1;
    }

    bool isFull() const {
        return this->top + 1 == this->capacity;
    }
};

bool isValid(const std::string& s) {
    Stack stack;
    for (char c : s) {
        switch (c) {
            case '(': case '{': case '[':
                stack.push(c);
                break;
            case ')':
                if (stack.isEmpty() || stack.pop() != '(')
                    return false;
                break;
            case '}':
                if (stack.isEmpty() || stack.pop() != '{')
                    return false;
                break;
            case ']':
                if (stack.isEmpty() || stack.pop() != '[')
                    return false;
                break;
        }
    }
    return stack.isEmpty();
}

int main(){
    cout << isValid("[[{()");
    return 0;
}