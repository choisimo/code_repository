//
// Created by csimo on 3/21/2024.
//


/**
 * infix
 * stack 이 비었을 경우, 연산자 추가
 * stack 이 비어있지 않을 경우, 연산자 우선순위 비교 후 수행
 * 우선순위가 더 큰게 위에
 *
 * */
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
    string postfix;
    string infix;
    stack<char> stack1;
public:
    calculator(int capacity): stack1(capacity){}
    int Op_priority(char op){
        switch(op) {
            case '*' :
            case '/' : return 3;
            case '+' :
            case '-' : return 2;
            default  : return 1;
        }
    }
    void infixToPostfix(string &infix){
        for (int i = 0; i < infix.length(); i++){
            if ('0' <= infix[i] && infix[i] <= '9'){
                postfix += infix[i];
            }
            else if (infix[i] == '('){
                stack1.push(infix[i]);
            }
            else if(infix[i] == ')'){
                while(!stack1.isEmpty() && stackMatch(stack1.peek(), infix[i])){
                    postfix += stack1.pop();
                }
                stack1.pop();
            }
            else {
                while(!stack1.isEmpty() && checkOpp(infix[i])) {
                    postfix += stack1.pop();
                }
                stack1.push(infix[i]);
            }
        }
        while(!stack1.isEmpty()){
            postfix += stack1.pop();
        }
        cout << " infix to prefix : " << postfix << endl;
    }
    bool stackMatch(char start, char end){
        return (start == '(' && end == ')');
    }
    bool checkOpp(char &ch){
        if (Op_priority(stack1.peek()) <= Op_priority(ch)) {
            return true;
        }
        return false;
    }
};
int main(){
    calculator calc(100);
    string infix;
    getline(std::cin,infix);
    calc.infixToPostfix(infix);
    return 0;
}
