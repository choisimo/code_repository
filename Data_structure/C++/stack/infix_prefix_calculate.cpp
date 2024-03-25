//
// Created by csimo on 3/21/2024.
//
#include <iostream>
#include <stdexcept>

using namespace std;

class myStack{
private:
    int *array;
    int capacity;
    int top;
public:
    myStack(int capacity){
        this->array = new int[capacity];
        this->capacity = capacity;
        this->top = -1;
    }

    ~myStack(){
        delete[] array;
    }

    bool isEmpty() const{
        return top == -1;
    }
    bool isFull() const{
        return top == capacity -1;
    }

    bool push(int item){
        if (isFull()) {
            return false;
        }
        //값을 넣기 전에 미리 배열 크기 늘리기
        array[++top] = item;
        return true;
    }

    int pop(){
        if (isEmpty()) {
            throw out_of_range("stack underflow");
        }
        // 데이터 제거 후 top 변수 감소
        return array[top--];
    }

    int peek(){
        if (isEmpty()) {
            throw out_of_range("stack underflow");
        }
        return array[top];
    }

    int size() const{
        return top + 1;
    }
};

int main(){
    myStack stack1(100);

    return 0;
}
