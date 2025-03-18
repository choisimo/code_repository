//
// Created by csimo on 10/10/2024.
//

#include <iostream>
using namespace std;

int main(){
    cout << "입력할 정수의 개수는?";
    int n;
    cin >> n;
    if (n <= 0)return 0;

    int *p = new int[n];
    if (!p){
        cout << "메모리 할당 불가능";
        return 0;
    }

    for (int i = 0; i < n; i++) {
        cout << i+1 << "번 째 정수 : " <<
    }
}