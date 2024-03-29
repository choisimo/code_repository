//
// Created by csimo on 3/24/2024.
//
#include <iostream>

using namespace std;
template <typename T>

class stack{
private:
    T* arr;
    int capacity;
    int top;
public:
    explicit stack(int capacity): capacity(capacity), top(-1){
        arr = new T[capacity];
    }

    ~stack(){
        //std::cout << "stack 동적할당 메모리 해제" << std::endl;
        delete[] arr;
    }

    bool empty() const{
        return top == -1;
    }
    bool full() const{
        return top == capacity -1;
    }
    bool push(T item){
        if(full()){
            return false;
        }
        arr[++top] = item;
        return true;
    }
    T pop(){
        if(empty()){
            throw std::out_of_range("stack underflow");
        }
        return arr[top--];
    }
    T peek(){
        if (empty())
        {
            throw std::out_of_range("stack underflow");
        }
        return arr[top];
    }
};

int main(){
    stack<int> stack1(10);
    for (int i = 0; i < 10; i++){
        stack1.push(i);
    }
    stack1.push(55);
    for(int i = 0; i < 10; i++){
        cout << i <<  "번 : " << stack1.pop() << endl;
    }
    return 0;
}
