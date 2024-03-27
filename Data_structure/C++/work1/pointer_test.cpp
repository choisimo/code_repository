//
// Created by csimo on 3/27/2024.
//

#include <iostream>

using namespace std;

int main(){
    int *arrayList = new int[10];
    for (int i = 0; i < 10; i++) {
        arrayList[i] = i;
    }
    cout << arrayList[0] << endl;
    cout << *arrayList << endl;
    cout << *(arrayList + 1) << endl;
    cout << &arrayList << endl;
    cout << &*arrayList << endl;
}