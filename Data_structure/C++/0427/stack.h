#include <stdexcept>
#include <iostream>

//
// Created by csimo on 4/27/2024.
//

template <typename T>
class Stack{
private:
    T *array;
    int capacity;
    int top;
public:
    explicit Stack(int capacity) : capacity(capacity), top(-1){
        array = new T[capacity];
    }

    ~Stack(){
        delete[] array;
    }

    bool isFull() const{
        return top == capacity - 1;
    }
    bool isHalf() const{
        return top <= capacity / 2;
    }
    bool isEmpty() const{
        return top == -1;
    }

    void push(T item){
        if (this->isFull()) {
            // throw std::out_of_range("Stack overflow");
            this->increaseCapacity();
        }
        array[++top] = item;
    }

    void pop(){
        if (this->isEmpty()){
            throw std::out_of_range("Stack underflow");
        } else if (this->isHalf() && capacity > 1) {
            this->decreaseCapacity();
        }
        return array[top--];
    }

    T peek(){
        if (this->isEmpty()){
            throw std::out_of_range("Stack underflow");
        }
        return array[top];
    }

    int getSize() const{
        return top + 1;
    }

    int getCapacity() const {
        return capacity;
    }

    void increaseCapacity(){
        int newCapacity = capacity * 2;
        T *newArray = new T[newCapacity];

        for (int i = 0; i < capacity; i++) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }
    void decreaseCapacity(){
        int newCapacity = capacity /2 + 1;
        T *newArray = new T[newCapacity];

        for(int i = 0; i < capacity; i++){
            newArray[i] = array[i];
        }

        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }
};
