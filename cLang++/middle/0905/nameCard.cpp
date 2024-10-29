//
// Created by csimo on 9/5/2024.
//

#include<iostream>
#include <iomanip>

class printMyName {
private:
    const char* nameValue;
    const char* phoneNum;
    const char* associatedTo;
    const char* hobbit;
public:
    printMyName(const char* n, const char* p, const char* a, const char* h): nameValue(n), phoneNum(p), associatedTo(a), hobbit(h){
    };

    void printNameTitle(){
        if (this->nameValue == NULL || this->phoneNum == NULL || this->associatedTo == NULL || this->hobbit == NULL) {
            std::runtime_error("필요한 정보들이 충족되지 않았습니다.\n");
        } else {
            std::cout << "┌────────────────────────────────────┐" << std::endl;
            std::cout << "│ " << this->nameValue << std::setw(32) << "│" << std::endl;
            std::cout << "│ 전화번호 : " <<  this->phoneNum << std::setw(14) << "│" << std::endl;
            std::cout << "│ 소속 : " <<  this->associatedTo << std::setw(4) << "│" << std::endl;
            std::cout << "│ 취미 : " <<  this->hobbit << std::setw(25) << "│" << std::endl;
            std::cout << "└────────────────────────────────────┘" << std::endl;
        }
    }
};

int main(int argc, char* argv[]){
    printMyName printN("최시몬", "010-1234-1324", "전북대 공과대학 컴퓨터공학과", "잠자기");
    printN.printNameTitle();
    return 0;
}