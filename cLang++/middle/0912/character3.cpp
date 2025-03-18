/*
    Created by csimo on 9/12/2024.
    공백을 포함한 문자열 입력 -> cin.getline() 사용
    cin.getline(char buf[], int size, char demitChar)
 */

#include <iostream>
#include <cstring>

using namespace std;

class addr {
private:
    int size_of_addr;
    char buffer[];
    char delim;
public:
    addr(int s, char* b, char d) : size_of_addr(s), buffer(b),
};

int main(){

    cout << "주소를 입력하세요" << endl;

    char address[100];
    cin.getline(address, 100, '\n');

    cout << "주소는 " << address << "입니다 \n" << endl;
}
