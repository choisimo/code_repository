//
// Created by csimo on 10/8/2024.
//
#include <iostream>
using namespace std;

int main(){
    int *p = new int();

    if (!p){
        cout << "메모리 할당 불가능" << endl;
        return 0;
    }

    *p = 5;
    int n = *p;
    cout << "*p = " << *p << endl;
    cout << "n = " << n << endl;


}