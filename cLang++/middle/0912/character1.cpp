/*
    Created by csimo on 9/12/2024.
    project STRCMP ��� ���
    int strcmp (const char * str1, const char * str2); // �� ���ڿ� �� �ϱ�
*/

#include <iostream>
#include <cstring>
#include "password.h"

using namespace std;



int main(){
    char password[11];

    cout << "���α׷��� �����Ϸ��� ��ȣ�� �Է��Ͻÿ�" << endl;

    while ((5 - pass_count) != 0) {
        cout << "��ȣ>>";
        cin >> password;
        if (strcmp(password, pass) == 0){
            cout << "��й�ȣ ��ġ!" << " : true" << endl;
            break;
        } else {
            pass_count++;
            cout << "��й�ȣ�� ��ġ ���� �ʽ��ϴ�. �ٽ� �Է��ϼ���! " << "���� ���� count : " << pass_count << endl;
            cout << "5ȸ ���н� ���� �Ұ� �մϴ�! " << "���� count : " << 5 - pass_count <<  endl;
            continue;
        }
    }
}
