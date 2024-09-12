//
// Created by csimo on 9/12/2024.
// 2장 레포트 3번 문제
//

/*
 *  ch2 - no : 3
 *  키보드로부터 두개의 정수를 읽어 큰 수를 화면에 출력하라.
 * */

#include <iostream>
using namespace std;

int main() {
    int n1, n2;
    cout << "두 수를 입력하라>>";
    cin >> n1 >> n2;

    int bignum = (n1 > n2 ? n1 : n2);
    cout << "큰수 = " << bignum << endl;
}