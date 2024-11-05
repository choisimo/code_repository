//
// Created by nodove on 24. 11. 5.
//
#include <iostream>
using namespace std;

int big(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int big(int arr[], int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (max < arr[i]) {
            max = arr[i];
        }
    }
    return max;
}

int main(){
    int array[] = {51, 22, 13, 34, 56};
    cout << big(3, 5) << endl;
    cout << big(array, 5) << endl;
    return 0;
}