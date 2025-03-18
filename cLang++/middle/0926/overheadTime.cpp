//
// Created by csimo on 9/26/2024.
//

#include <iostream>
using namespace std;

int odd(int x){
    return (x % 2);
}

int main(){
    int sum = 0;

    for (int i = 1; i <= 1000000; i++) {
        if (odd(i)){
            sum += i;
        }
    }
    cout << sum;
}