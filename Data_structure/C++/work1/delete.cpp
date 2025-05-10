//
// Created by csimo on 3/21/2024.
//
#include <iostream>

using namespace std;

int main(){
    int *p = new int[5];
    for(int i = 0; i < sizeof(p); i++){
        p[i] = i;
    }
    delete p;
    for(int i = 1; i < 5; i++){
        cout << p << endl;
    }
}