//
// Created by csimo on 3/29/2024.
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
#include <cstring>

using namespace std;

template<typename T> class stack{
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
            T *newarr = new T[capacity *2];
            memcpy(newarr, arr, capacity * sizeof(T));
            delete[] arr;
            arr = newarr;
            capacity *= 2;
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
    stack<double> stack2;
    bool isError = false;
public:
    calculator(int capacity): stack1(capacity), stack2(capacity){}
    int Op_priority(char op){
        switch(op) {
            case '*' :
            case '/' : return 3;
            case '+' :
            case '-' : return 2;
            default  : return 1;
        }
    }
    void infixToPostfix(const string &infix){
        for (int i = 0; i < infix.length(); i++){
            if (infix[i] == ' '){
                continue;
            }
            if (infix[i] == '/' && infix[i+2] == '0'){
                cout << "Error : zero division error\n"<< endl;
                isError = true;
                return;
            }
            if (('0' <= infix[i] && infix[i] <= '9') || infix[i] == '.'){
                string num;
                while(('0' <= infix[i] && infix[i] <= '9') || infix[i] == '.'){
                    num += infix[i];
                    i++;
                }
                i--;
                postfix += num + " ";
            }
            else if (infix[i] == '(' && !stack1.isFull()){
                stack1.push(infix[i]);
            }
            else if(infix[i] == ')'){
                while(!stack1.isEmpty() && stack1.peek() != '('){
                    postfix += stack1.pop();
                }
                if (!stack1.isEmpty()) stack1.pop();
                continue;
            }
            else {
                while(!stack1.isEmpty() && checkOpp(infix[i])) {
                    postfix += stack1.pop();
                    postfix += " ";
                }
                stack1.push(infix[i]);
            }
        }
        while(!stack1.isEmpty()){
            postfix += stack1.pop();
        }
        //cout << postfix << endl;
    }

    bool checkOpp(const char &ch){
        if (Op_priority(ch) <= Op_priority(stack1.peek())) {
            return true;
        }
        return false;
    }

    void postfixResult() {
        double postfix_calc_result = 0;
        string temp;
        if (isError == true){
            return;
        }
        for (char c : postfix){
            // 숫자이거나 소수점 이라면...
            if (isdigit(c) || c == '.'){
                temp += c;
            }
            // 공백이라면
            else if (c == ' ') {
                if (!temp.empty() && !stack2.isFull()) {
                    stack2.push(stod(temp));
                    temp.clear();
                }
            } else {
                if(stack2.isEmpty()){
                    cout << "stack is empty! : " << c << endl;
                    return;
                }
                double val2 = stack2.pop();
                double val1 = stack2.pop();
                switch(c) {
                    case '+' : stack2.push(val1 + val2); break;
                    case '-' : stack2.push(val1 - val2); break;
                    case '*' : stack2.push(val1 * val2); break;
                    case '/' :
                        if (val2 == 0){
                            cout << "Error : zero division error\n";
                            return;
                        }
                        stack2.push(val1 / val2); break;
                }
            }
        }
        if (!temp.empty()){
            stack2.push(stod(temp));
        }
        postfix_calc_result = stack2.pop();
        cout << fixed;
        cout.precision(2);
        cout << postfix_calc_result << endl;
    }
};
int main() {
    calculator calc(100);
    string infix;
    getline(std::cin, infix);
    calc.infixToPostfix(infix);
    calc.postfixResult();
    return 0;
}