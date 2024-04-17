//
// Created by csimo on 4/17/2024.
//


#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;
template <typename T>
class Queue{
private:
    T* array;
    int capacity;
    int front_index;
    int rear_index;
public:
    void resize(){
        int newCapacity = capacity * 2;
        T* newArray = new T[newCapacity];

        for(int i = 0; i < this->size(); ++i){
            newArray[i] = array[(front_index + i) % capacity];
        }

        delete[] array;
        array = newArray;
        capacity = newCapacity;
        front_index = 0;
        rear_index = this->size() -1;
    }
    Queue(int capacity1) : capacity(capacity1), front_index(0), rear_index(-1){
        array = new T[capacity];
    }
    ~Queue(){
        delete[] array;
    }
    void enqueue(const T &value){
        if (this -> size() == capacity){
            resize();
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
        if (rear_index >= front_index) {
            return rear_index - front_index + 1;
        } else {
            return (rear_index + capacity - front_index + 1) % capacity;
        }
    }
    int getCapacity() const {
        return this-> capacity;
    }
};


class process{
private:
    Queue<int> orders;
    vector<int> lifes;
public:
    process(int N, int K, int D) : orders(N), lifes(N, D){
        for (int i = 0; i < N; i++){
            orders.enqueue(i);
        }
        while(!orders.isEmpty()){
            for (int i = 0; i < K - 1; i++){
                orders.enqueue(orders.dequeue());
            }
            int K_person = orders.dequeue();
            lifes[K_person] = lifes[K_person] - 1;

            if (lifes[K_person] > 0){
                orders.enqueue(K_person);
            } else {
                cout << K_person << " is dead" << endl;
            }
        }
    }

};

int main() {
    int N, K, D;
    cin >> N >> K >> D;
    process p(N, K, D);
    return 0;
}