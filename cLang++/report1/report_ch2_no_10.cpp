//
// Created by csimo on 9/12/2024.
//


/*
 * ch 2 - no 10
 * ���ڿ��� �ϳ� �Է¹ް� ���ڿ��� �κ� ���ڿ���
 * ������ ���� ����ϴ� ���α׷��� �ۼ��϶�.
 * */
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

int main() {
    char strValue[100];

    cout << "���ڿ� �Է�>>";
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