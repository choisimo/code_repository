//
// Created by csimo on 4/16/2024.
//


#include <stdexcept>

class DataStructure {
protected:
    int* array;
    int capacity;
    int ptr;
protected:
    void increaseCapacity() {
        int newCapacity = capacity * 2;  int* newArray = new int[newCapacity];
        for (int i = 0; i < capacity; i++) {  newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }
    void decreaseCapacity() {
        int newCapacity = capacity / 2 + 1;  int* newArray = new int[newCapacity];
        for (int i = 0; i < newCapacity; i++) {  newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }
public:
    explicit DataStructure(int capacity) : capacity(capacity) {  array = new int[capacity];
    }
    ~DataStructure() {
        delete[] array;
    }
    virtual void printElements() const {
        throw std::runtime_error("Not implemented");  }
    virtual bool isEmpty() const {
        throw std::runtime_error("Not implemented");  }
    virtual bool isFull() const {
        throw std::runtime_error("Not implemented");  }
    virtual void add(int item) {
        throw std::runtime_error("Not implemented");  }
    virtual int remove() {
        throw std::runtime_error("Not implemented");  }
    virtual int size() {
        throw std::runtime_error("Not implemented");  }
};

class Stack : public DataStructure{
    public Stack
    bool isEmpty() const override {

    }
};