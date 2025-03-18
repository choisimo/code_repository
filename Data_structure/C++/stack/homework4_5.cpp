//
// Created by csimo on 3/27/2024.
//
#include <iostream>
using namespace std;
template<typename T> class Stack {
private:
    T *array;
    int capacity;
    int top;
    int multiplier;
private:
    bool isFull() const {
        return top == capacity - 1;
    }

    bool isHalf() const {
        return top <= capacity / 2;
    }

    void increaseCapacity() {
        int newCapacity = capacity * multiplier;
        T *newArray = new T[newCapacity];
        for (int i = 0; i < capacity; i++)
            newArray[i] = array[i];
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

    void decreaseCapacity() {
        int newCapacity = capacity / multiplier + 1;
        T *newArray = new T[newCapacity];
        for (int i=0; i < newCapacity; i++)
            newArray[i] = array[i];
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

public:
    explicit Stack(int capacity, const int multiple) : capacity(capacity), top(-1), multiplier(multiple) {
        array = new T[capacity];
    }
    ~Stack() {
        delete[] array;
    }

    bool isEmpty() const {
        return top == -1;
    }

    void push(int item) {
        if (this->isFull()) {
            this->increaseCapacity();
        }
        array[++top] = item;
    }

    int pop() {
        if (this->isEmpty()) {
            throw std::out_of_range("Stack underflow");
        } else if (this->isHalf()) {
            this->decreaseCapacity();
        }
        return array[top--];
    }
    T peak() {
        if (isEmpty()) {
            throw std::out_of_range("Stack underflow");
        }
        return array[top];
    }

    int getSize() const {
        return top + 1;
    }

    int getCapacity() const {
        return capacity;
    }
    bool contains(int item) const{
        int i = 0;
        while (i < top){
            if(array[i] == item){
                return true;
            }
            i++;
};


int main() {
    Stack<int> stack(5, (3.2));
    for (int i = 0; i < 10; i++) {
        stack.push(i);
    }
    cout << stack.getCapacity() << endl;
    for (int i = 0; i < 10; i++) {
        stack.push(10 + i);
    }
    cout << stack.getCapacity();
    return 0;
}
