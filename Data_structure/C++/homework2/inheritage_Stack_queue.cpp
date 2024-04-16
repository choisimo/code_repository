//
// Created by csimo on 4/16/2024.
//


#include <stdexcept>
#include <iostream>

class DataStructure {
private:
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
    void setPtr(int ptr){
        this->ptr = ptr;
    };
    void setArray(int index, int item){
        if (index >= 0 && index < capacity){
            array[index] = item;
        }
    }
    int getPtr() const {
        return ptr;
    }
    int getCapacity() const{
        return capacity;
    }
    int getArray(int index) const {
        if (index >= 0 && index < capacity){
            return array[index];
        } else {
            throw std::out_of_range("out of range");
        }
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
public:
    explicit Stack(int capacity) : DataStructure(capacity){
        this->setPtr(-1);
    }
    bool isEmpty() const override {
        return this->getPtr() == -1;
    }
    bool isFull() const override{
        return this->getPtr() == getCapacity() -1;
    }
    void add (int item) override{
        if (isFull()){
            this->increaseCapacity();
        }
        setPtr(getPtr() + 1);
        setArray(getPtr(), item);
    }
    int remove() override{
        if(isEmpty()){
            throw std::runtime_error("stack underflow");
        } else if (getPtr() <= getCapacity()/2){
            this->decreaseCapacity();
        }
        int item = getArray(getPtr());
        setPtr(getPtr()-1);
        return item;
    }
    int size() override{
        return this->getPtr() + 1;
    }
    void printElements() const override{
        for (int i = 0; i <= getPtr(); i++){
            std::cout << getArray(i) << ", ";
        }
        std::cout << std::endl;
    };
    ~Stack(){};
};

class Queue: public DataStructure{
private:
    int front;
    int rear;
public:
    explicit Queue(int capacity) : DataStructure(capacity), front(0), rear(-1){}

    bool isEmpty() const override{
        return (front == rear + 1);
    }
    bool isFull() const override{
        return (rear == this->getCapacity() - 1);
    }
    void add(int item) override{
        if (isFull()){
            this->increaseCapacity();
        }
        setArray(++rear, item);
    }
    int remove() override{
        int total_size = rear - front;
        if (isEmpty()){
            throw std::runtime_error("Queue is Empty");
        } else if ((total_size) <= getCapacity() / 2){
            this->decreaseCapacity();
            front = 0;
            rear = total_size;
        }
        int item = getArray(front++);
        return item;
    }
    int size() override{
        return rear - front + 1;
    }
    void printElements() const override{
        for (int i = front; i <= rear; i++){
            std::cout << getArray(i) << ", ";
        }
        std::cout << std::endl;
    };
    ~Queue(){};
};