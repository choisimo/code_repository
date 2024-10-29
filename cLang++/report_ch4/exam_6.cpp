/*
* 문제 6번.
 * string 클래스를 이용하여 사용자가 입력한 영어 한 줄을 문자열로
 * 입력받고 거꾸로 출력하는 프로그램을 작성하시오.
 * @author : simon choi
 * @date   : 24-10-29
 */

#include <cstring>
#include <iostream>
using namespace std;

int main() {
    string str;
    cout << "아래에 영어 한 줄을 입력 (exit 입력 시 종료) : " << endl;

    while (true)
    {
        cout << " >> ";
        getline(cin, str, '\n');
        if (str == "exit") break;

        for (int i = str.size(); i >= 0; i--)
        {
            cout << str[i];
        } cout << endl;
    }
}