//
// Created by csimo on 3/21/2024.
//
#include <iostream>
#include <stdexcept>

using namespace std;
template<typename T>

class stack{
private:
    T *arr;
    int capacity;
    int top;
public:
    stack(int capacity){
        this->arr = new T[capacity];
        this->capacity = capacity;
        this->top = -1;
    }

    ~stack(){
        delete[] arr;
    }

    bool isEmpty() const{
        return top == -1;
    }
    bool isFull() const{
        return top == capacity -1;
    }

    bool push(T item){
        if (isFull()) {
            return false;
        }
        //값을 넣기 전에 미리 배열 크기 늘리기
        arr[++top] = item;
        return true;
    }

    T pop(){
        if (isEmpty()) {
            throw out_of_range("stack underflow");
        }
        // 데이터 제거 후 top 변수 감소
        return arr[top--];
    }

    T peek(){
        if (isEmpty()) {
            throw out_of_range("stack underflow");
        }
        return arr[top];
    }

    int size() const{
        return top + 1;
    }
};

class calculator{
private:
    stack<char> stack1;
public:
    explicit calculator(int capacity): stack1(capacity){}
    int Op_priority(char &op){
        switch(op) {
            case '*' :
            case '/' : return 1;
            case '+' :
            case '-' : return 2;
            default  : return 3;
        }
    }
};
int main(){
    std::string Str;
    calculator c(100);
    std::getline(std::cin,Str);
    std::cout << Str << std::endl;
    for (int i = 0; i < Str.length(); i++){
        std::cout << Str[i] << ", ";
    }
    return 0;
}
