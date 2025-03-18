/*
    Created by csimo on 9/12/2024.
    project STRCMP 사용 방법
    int strcmp (const char * str1, const char * str2); // 두 문자열 비교 하기
*/

#include <iostream>
#include <cstring>
#include "password.h"

using namespace std;



int main(){
    char password[11];

    cout << "프로그램을 종료하려면 암호를 입력하시오" << endl;

    while ((5 - pass_count) != 0) {
        cout << "암호>>";
        cin >> password;
        if (strcmp(password, pass) == 0){
            cout << "비밀번호 일치!" << " : true" << endl;
            break;
        } else {
            pass_count++;
            cout << "비밀번호가 일치 하지 않습니다. 다시 입력하세요! " << "현재 실패 count : " << pass_count << endl;
            cout << "5회 실패시 접근 불가 합니다! " << "남은 count : " << 5 - pass_count <<  endl;
            continue;
        }
    }
}
