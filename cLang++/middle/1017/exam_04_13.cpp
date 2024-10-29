//
// Created by csimo on 10/17/2024.
//
#include <iostream>
#include <string>
using namespace std;

int main(){
    string* s = new string();

    cout << "문자열을 입력하세요 (한글 안됨)" << endl;
    getline(cin, *s, '\n');
    int len = s->length();

    for (int i = 0; i < len; i++) {
        string first = s->substr(0, 1);
        string sub = s->substr(1, len - 1);
        *s = sub + first;
        cout << *s << endl;
    }
    delete s;
}