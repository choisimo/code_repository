/*
    Created by csimo on 9/12/2024.
    ������ ������ ���ڿ� �Է� -> cin.getline() ���
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

    cout << "�ּҸ� �Է��ϼ���" << endl;

    char address[100];
    cin.getline(address, 100, '\n');

    cout << "�ּҴ� " << address << "�Դϴ� \n" << endl;
}
