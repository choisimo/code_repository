//
// Created by csimo on 5/13/2024.
//
#include <iostream>
class Stack {
private:
    int *array;
    int capacity;
    int top;
private:
    bool isFull() const {
        return top == capacity;
    }

    bool isHalf() const {
        return top <= capacity / 2;
    }

    void increaseCapacity() {
        int newCapacity = capacity * 2;
        int *newArray = new int[newCapacity];
        for (int i = 0; i < capacity; i++)
            newArray[i] = array[i];
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

    void decreaseCapacity() {
        int newCapacity = capacity / 2 + 1;
        int *newArray = new int[newCapacity];
        for (int i = 0; i < newCapacity; i++)
            newArray[i] = array[i];
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

public:
    Stack(int capacity) : capacity(capacity), top(0) {
        array = new int[capacity];
    }

    ~Stack() {
        delete[] array;
    }

    bool isEmpty() const {
        return top == 0;
    }

    void push(int item) {
        if (this->isFull()) {
            this->increaseCapacity();
        }
        array[top++] = item;
    }

    int pop() {
        if (this->isEmpty()) {
            throw std::out_of_range("Stack underflow");
        } else if (this->isHalf()) {
            this->decreaseCapacity();
        }
        return array[--top];
    }

    int peak() {
        if (isEmpty()) {
            throw std::out_of_range("Stack underflow");
        }
        return array[top - 1];
    }

    int getSize() const {
        return top + 1;
    }

    int getCapacity() const {
        return capacity;
    }

    bool contains(int item) const {
        int index = 0;
        while (index < getSize()){
            if (array[index] == item){
                return true;
            }
            index++;
        }
        return false;
    }
};