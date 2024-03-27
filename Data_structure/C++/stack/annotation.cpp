//
// Created by csimo on 3/21/2024.
//
/**
 * line_count, bracket_count (��ȣ ¦ ��)
 * �ּ� ���� //, /**./
 * ���� ���� \n
 * �ּ� ���� bracket ����, �ּ� �ܺ� bracket ��Ī üũ -> �ȵǸ� ����
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
        std::cout << "stack �����Ҵ� �޸� ����" << std::endl;
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
        std::cout << " getTop ȣ�� : " << arr[this->top] << std::endl;
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
            // �ּ� /*
            if(!in_literal_line && !in_annotation_line && Str[i] == '/' && Str[i+1] == '*' && i < Str.length() - 1){
                std::cout << "/* ����" << std::endl;
                in_annotation_line = true;
                ++i;
                continue;
            }
            // �ּ� */
            if(in_annotation_line && Str[i] == '*' && Str[i+1] == '/' && i < Str.length() - 1){
                std::cout << "*/ ��" << std::endl;
                in_annotation_line = false;
                ++i;
                continue;
            }

            if(!in_literal_line && !in_annotation_line && Str[i] == '"' && i < Str.length() - 1){
                std::cout << "literal, " << Str[i] << " ���� " << std::endl;
                in_literal_line = true;
                continue;
            }
            if(in_literal_line && Str[i] == '"' && i < Str.length() - 1){
                std::cout << "literal end, " << Str[i] << " " << std::endl;
                in_literal_line = false;
                continue;
            }

            if(Str[i] == '\n'){
                std::cout << " �� �ٲ� ī��Ʈ 1����" << std::endl;
                line_count++;
            }

            if (!in_annotation_line && !in_literal_line){
                // �ּ� //�� \n ���� ������
                if(Str[i] == '/' && Str[i+1] == '/'){
                    std::cout << "// ����" << std::endl;
                    while(Str[i] != '\n' && i < Str.length()){
                        ++i;
                    }
                    // �� �ٲ� ī��Ʈ �߰�
                    line_count++;
                }

                if (Str[i] == '(' || Str[i] == '{' || Str[i] == '[')
                {
                    std::cout << Str[i] << " ���� " << std::endl;
                    stack1.push(Str[i]);
                }
                else if ((Str[i] == ')' || Str[i] == '}' || Str[i] == ']')
                &&!stack1.empty() && checkBracket(stack1.getTop(), Str[i]))
                {
                    std::cout << Str[i] << " ���� " << std::endl;
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
