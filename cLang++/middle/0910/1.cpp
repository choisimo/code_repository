//
// Created by csimo on 9/10/2024.
//
#include <iostream>

class test1 {
private:
    char* String1;
public:
    test1(char* Text1): String1(Text1){
        std::cout << this->String1 << std::endl;

    }
};

int main()
{
    test1 t1("hello");
}