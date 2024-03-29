//
// Created by csimo on 3/27/2024.
//
#include <iostream>
using namespace std;
class Stack {
private:
    int *array;
    int capacity;
    int top;
private:
    bool isFull() const {
        return top == capacity - 1;
    }
    bool isHalf() const {
        return top <= capacity / 2;
    }
    void increaseCapacity() {
        int newCapacity = capacity * 2;
        int *newArray = new int[newCapacity];
        for (int i=0; i<capacity; i++)
            newArray[i] = array[i];
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }
    void decreaseCapacity() {
        int newCapacity = capacity / 2 + 1;
        int *newArray = new int[newCapacity];
        for (int i=0; i < newCapacity; i++)
            newArray[i] = array[i];
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }
public:
    Stack(int capacity) : capacity(capacity), top(-1) {
        array = new int[capacity];
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
    int peak() {
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
    bool contains(int item) const {
        if(!isEmpty()){
            if(item == *(array + top))
            {
                cout << "contains true" << endl;
                return true;
            }
            else {
                cout << "*array : " << *array << endl;
                cout << "*(array+top) : " << *(array + top) << endl;
                cout << "*(array+top-1) : " << *(array + top -1) << endl;
                array[*(array + top)] = array[*(array + top -1)];
                cout << "contains ~ing" << endl;
                contains(item);
            }
        } else {
            cout << "contains false" << endl;
            return false;
        }
    }
};

int main(){
    Stack stack(10);
    for (int i = 0; i < 10; i++) {
        stack.push(i);
        //cout <<  i << "insert, stack peek : " <<  stack.peak() << endl;
    }
    stack.contains(5);
    stack.contains(11);
}