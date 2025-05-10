//
// Created by csimo on 6/29/2024.
//
#include <iostream>

class calculate{
private:
    int num1;
    int num2;
public:
    calculate(int n1): num1(n1), num2(9){}
    ~calculate();
    void printResult(){
        int n1 = num1; int n2 = num2;
        std::cout << "==================================================" << std::endl;
        std::cout << "===================| ( " << n1 << "X" << n2 <<  ") |=====================" << std::endl;
        std::cout << "==================================================" << std::endl;
        for(int i = 1; i < n1 + 1; i++) {
            std::cout << i << " th mul : " << " ";
            for(int j = 1; j < n2 + 1; j++){
                if(j == n2){
                    std::cout << i * j << std::endl;
                } else {
                    std::cout << i * j << ", ";
                }
            }
            std::cout << "\n";
        }
    }
};

int main(int argc, char* argv[]){
    std::cout << "input num to multiply from 1 to 9 : ";
    int input_num = -1;
    std::cin >> input_num;
    calculate* cal = new calculate(input_num);
    cal->printResult();
}