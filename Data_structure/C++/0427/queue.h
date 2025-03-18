//
// Created by csimo on 4/27/2024.
//
#ifndef QUEUE_H
#define QUEUE_H


#include <stdexcept>

template<typename T>
class Queue{
private:
    T* array;
    int capacity;
    int front_index;
    int rear_index;
public:
    Queue(int capacity = 8) : capacity(capacity), front_index(0), rear_index(-1){
        array = new T[capacity];
    }
    ~Queue(){
        delete[] array;
    }

    void enqueue(const T& value){
        if (this->size() == capacity){
            this->resize();
        }
        rear_index = (rear_index + 1) % capacity;
        array[rear_index] = value;
    }

    T dequeue(){
        if (isEmpty()){
            throw std::out_of_range("queue is empty");
        }
        T value = array[front_index++];
        return value;
    }

    T front() const {
        if (isEmpty()){
            throw std::out_of_range("queue is empty");
        }
        return array[front_index];
    }

    bool isEmpty() const {
        return this->size() == 0;
    }

    int size() const {
        return this->rear_index - this->front_index + 1;
    }

    int getCapacity() const {
        return this->capacity;
    }

    void resize(){
        int newCapacity = capacity * 2;
        T *newArray = new T[newCapacity];

        for (int i = 0; i < this->size(); ++i){
            newArray[i] = array[(front_index + i) % capacity];
        }

        delete[] array;
        array = newArray;
        capacity = newCapacity;
        front_index = 0;
        rear_index = this->size() - 1;
    }
    void decreaseSize(){
        int newCapacity = capacity / 2 + 1;
        T *newArray = new T[newCapacity];

        for (int i = 0; i < this->size(); ++i){
            newArray[i] = array[(front_index + i) % capacity];
        }

        delete[] array;
        array = newArray;
        capacity = newCapacity;
        front_index = 0;
        rear_index = this->size() - 1;
    }
};

#endif