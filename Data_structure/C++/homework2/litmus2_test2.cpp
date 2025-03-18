#include <iostream>
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
    explicit Queue(int capacity1) : capacity(capacity1), front_index(0), rear_index(-1){
        array = new T[capacity];
    }

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

    ~Queue(){
        delete[] array;
    }
    void enqueue(const T &value){
        if (this -> size() == capacity -1){
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
    vector<int> lifes;
    Queue<int> player_orders;
    vector<int> delete_orders;
public:
    process(int N, int K, int D) : player_orders(N),lifes(N, D){
        task(N, K);
    }

    void task(int N, int K){
        for (int i = 0; i < N; i++){
            player_orders.enqueue(i);
        }
        while(!player_orders.isEmpty()){
            for(int i = 0; i < K - 1; i++){}
            player_orders.enqueue(player_orders.front());
            player_orders.dequeue();
        }

        int K_order = player_orders.front();
        player_orders.dequeue();
        lifes[K_order]--;

        if (lifes[K_order] > 0){
            player_orders.enqueue(K_order);
        } else {
            delete_orders.push_back(K_order + 1);
        }
    }

    void printResult(){
        cout << "<";
        for (size_t i = 0; i < delete_orders.size(); i++){
            cout << delete_orders[i];
            if (i < delete_orders.size() -1)
                cout << ", ";
        }
        cout << ">" << endl;
    }

};
int main() {
    int N, K, D;
    cin >> N >> K >> D;
    process p(N, K, D);
    p.printResult();
    return 0;
}