//
// Created by csimo on 10/17/2024.
//
#include <iostream>
#include <string>
using namespace std;

int main(){
    std::string names[5];

    for (int i = 0; i < 5; i++) {
        cout << "이름 >>";
        getline(cin, names[i], '\n');
    }

    string num1 = "55155";
    int num2 = stoi(num1);
    cout << "test stoi : " << num1 << " to " << num2 << endl;

    string latter = names[0];
    for (int i = 1; i < 5; i++) {
        if (latter < names[i]) {
            latter = names[i];
        }
    }
    cout << "사전에서 가장 뒤에 나오는 문자열은 " << latter << endl;
}