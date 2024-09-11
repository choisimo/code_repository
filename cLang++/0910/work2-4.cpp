//
// Created by csimo on 9/10/2024.
//
#include <iostream>
using namespace std;

int main(){
    cout << "이름을 입력하시오 >> ";

    // 한글은 5개, 영문은 10까지 (한글은 2bit, 영문은 1bit)
    char name[11];
    cin >> name;

    // 공백을 읽는지 테스트 해보기 (마이클 -> 마 이클 비교)
    cout << "이름은" << name << "입니다" << endl;
    // <notation> cin 은 공백까지만 읽는다!
}