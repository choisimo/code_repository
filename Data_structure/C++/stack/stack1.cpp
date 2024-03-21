//
// Created by csimo on 3/20/2024.
//

using namespace std;

#include <iostream>
class STACK{
private:
    int* arr;
    int capacity;
    int top;
public:

    STACK(int capacity) : capacity(capacity), top(-1){
        arr = new int[capacity];
    }
    ~STACK(){
        delete[] arr;
    }

    bool isEmpty() const{
        return top == -1;
    }

    bool isFull() const{
        cout << "isFull executed" << endl;
        return top == capacity - 1;
    }
};

int main(){
    STACK stack(10);
    stack.isFull();
    return 0;
}