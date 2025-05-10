//
// Created by csimo on 4/28/2024.
//
#include <iostream>
#include <stdexcept>

class Stack {
    char* array;
    int capacity;
    int top;

public:
    Stack(int capacity) : capacity(capacity), top(-1){
        array = new char[capacity];
    }
    ~Stack(){
        delete[] array;
    }

    void push(char item){
        if(top + 1 == capacity){
            throw std::out_of_range("stack overflow");
        }
        array[++top] = item;
    }

    char pop(){
        if(this->top == -1){
            throw std::out_of_range("stack underflow");
        }
        return array[top--];
    }

    bool isEmpty() const{
        return top == -1;
    }

    void clear() {
        this->top == -1;
    }
};

