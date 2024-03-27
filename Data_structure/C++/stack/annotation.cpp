//
// Created by csimo on 3/21/2024.
//
/**
 * line_count, bracket_count (괄호 짝 수)
 * 주석 종류 //, /**./
 * 라인 구분 \n
 * 주석 내부 bracket 무시, 주석 외부 bracket 매칭 체크 -> 안되면 오류
 *
 */

#include <iostream>
#include<stdexcept>
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
        std::cout << "stack 동적할당 메모리 해제" << std::endl;
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

    T getTop(){
        if(this->top == -1){
            throw std::out_of_range("stack is empty");
        }
        std::cout << " getTop 호출 : " << arr[this->top] << std::endl;
        return arr[this->top];
    }

};

class check{
private:
    int line_count = 0;
    int bracket_count = 0;
    bool in_annotation_line = false;
    bool in_literal_line = false;
    stack<char> stack1;
public:
    explicit check(int capacity) : stack1(capacity){}
    void checkAnnotation(const std::string& Str){
        for (int i = 0; i < Str.length(); i++) {
            // 주석 /*
            if(!in_literal_line && !in_annotation_line && Str[i] == '/' && Str[i+1] == '*' && i < Str.length() - 1){
                std::cout << "/* 시작" << std::endl;
                in_annotation_line = true;
                ++i;
                continue;
            }
            // 주석 */
            if(in_annotation_line && Str[i] == '*' && Str[i+1] == '/' && i < Str.length() - 1){
                std::cout << "*/ 끝" << std::endl;
                in_annotation_line = false;
                ++i;
                continue;
            }

            if(!in_literal_line && !in_annotation_line && Str[i] == '"' && i < Str.length() - 1){
                std::cout << "literal, " << Str[i] << " 시작 " << std::endl;
                in_literal_line = true;
                continue;
            }
            if(in_literal_line && Str[i] == '"' && i < Str.length() - 1){
                std::cout << "literal end, " << Str[i] << " " << std::endl;
                in_literal_line = false;
                continue;
            }

            if(Str[i] == '\n'){
                std::cout << " 줄 바꿈 카운트 1증가" << std::endl;
                line_count++;
            }

            if (!in_annotation_line && !in_literal_line){
                // 주석 //은 \n 나올 때까지
                if(Str[i] == '/' && Str[i+1] == '/'){
                    std::cout << "// 만남" << std::endl;
                    while(Str[i] != '\n' && i < Str.length()){
                        ++i;
                    }
                    // 줄 바꿈 카운트 추가
                    line_count++;
                }

                if (Str[i] == '(' || Str[i] == '{' || Str[i] == '[')
                {
                    std::cout << Str[i] << " 시작 " << std::endl;
                    stack1.push(Str[i]);
                }
                else if ((Str[i] == ')' || Str[i] == '}' || Str[i] == ']')
                &&!stack1.empty() && checkBracket(stack1.getTop(), Str[i]))
                {
                    std::cout << Str[i] << " 종료 " << std::endl;
                    stack1.pop();bracket_count++;
                }
                else if (Str[i] == ')' || Str[i] == '}' || Str[i] == ']'){
                    std::cout << "stack not match" << std::endl;
                    printErrorResult();
                    return;
                }
            }
        }
        // bracket stack check
        if(stack1.empty()){
            printOKResult();
        } else {
            printErrorResult();
        }
    }
    void printOKResult() const{
        std::cout << "OK, ";
        std::cout << "Line_count : " << line_count;
        std::cout << ", ";
        std::cout << "bracket_count : " << bracket_count;
        std::cout << std::endl;
    }
    void printErrorResult() const{
        std::cout << "Error, ";
        std::cout << "Line_count : " << line_count;
        std::cout << ", ";
        std::cout << "bracket_count : " << bracket_count;
        std::cout << std::endl;
    }
    bool checkBracket(char start, char end){
        return (start == '(' && end == ')') ||
               (start == '{' && end == '}') ||
               (start == '[' && end == ']');
    }
};

void checkMatching(const std::string& Str){
    check check1(100);
    check1.checkAnnotation(Str);
}



int main(){
    std::string Str,temp;
    while(true){
        std::getline(std::cin,temp);
        if(temp=="EOF")break;
        Str.append(temp);
        Str.append("\n");
        std::cin.clear();
    }
    checkMatching(Str);

    return 0;
}
