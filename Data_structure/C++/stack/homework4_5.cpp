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
<<<<<<< HEAD
        for (int i=0; i<capacity; i++)
=======
        for (int i = 0; i < capacity; i++)
>>>>>>> f168c9db9b0a889efabe0b364206b37b59e9ca9b
            newArray[i] = array[i];
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

    void decreaseCapacity() {
<<<<<<< HEAD
        int newCapacity = capacity / multiplier + 1;
        T *newArray = new T[newCapacity];
        for (int i=0; i < newCapacity; i++)
=======
        int newCapacity = capacity / 2 + 1;
        T *newArray = new T[newCapacity];
        for (int i = 0; i < newCapacity; i++)
>>>>>>> f168c9db9b0a889efabe0b364206b37b59e9ca9b
            newArray[i] = array[i];
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

public:
<<<<<<< HEAD
    explicit Stack(int capacity, int multiple) : capacity(capacity), top(-1), multiplier(multiple) {
=======
    explicit Stack(int capacity, const int multiple) : capacity(capacity), top(-1), multiplier(multiple) {
>>>>>>> f168c9db9b0a889efabe0b364206b37b59e9ca9b
        array = new T[capacity];
    }
    ~Stack() {
        delete[] array;
    }

    bool isEmpty() const {
        return top == -1;
    }
<<<<<<< HEAD
    void push(T item) {
=======

    void push(int item) {
>>>>>>> f168c9db9b0a889efabe0b364206b37b59e9ca9b
        if (this->isFull()) {
            this->increaseCapacity();
        }
        array[++top] = item;
    }
<<<<<<< HEAD
    T pop() {
=======

    int pop() {
>>>>>>> f168c9db9b0a889efabe0b364206b37b59e9ca9b
        if (this->isEmpty()) {
            throw std::out_of_range("Stack underflow");
        } else if (this->isHalf()) {
            this->decreaseCapacity();
        }
        return array[top--];
    }
<<<<<<< HEAD
=======

>>>>>>> f168c9db9b0a889efabe0b364206b37b59e9ca9b
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
<<<<<<< HEAD
    bool contains(int item) const{
        int i = 0;
        while (i < top){
            if(array[i] == item){
                return true;
            }
            i++;
=======

    bool contains(int item) const {
        while(array[top] != item){
            
>>>>>>> f168c9db9b0a889efabe0b364206b37b59e9ca9b
        }
        return false;
    }
};

<<<<<<< HEAD
int main (int argc, char* argv[]) {
    Stack<int> stack(5, 3); // capacity = 5이며, multiplier = 3임.
    for (int i=0; i < 10; i++)
        stack.push(i);
    std::cout << stack.getCapacity() << std::endl; // 15가 출력되어야 함.
    for (int i=0; i < 10; i++)
        stack.push(i + 10);

    std::cout << stack.getCapacity() << std::endl; // 45가 출력되어야 함.
}
=======
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
>>>>>>> f168c9db9b0a889efabe0b364206b37b59e9ca9b
