//
// Created by csimo on 9/12/2024.
//


/*
 * ch 2 - no 10
 * 문자열을 하나 입력받고 문자열의 부분 문자열을
 * 다음과 같이 출력하는 프로그램을 작성하라.
 * */
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

int main() {
    char strValue[100];

    cout << "문자열 입력>>";
    cin.getline(strValue, 100, '\n');

    for (int i = 0; i < strlen(strValue); i++)
    {
        for (int j = 0; j <= i; j++)
        {
            cout << strValue[j];
        }
        cout << '\n';
    }
    return 0;
}