//
// Created by csimo on 10/8/2024.
//
#include <iostream>
using namespace std;

int main(){
    int a = 1;
    int *num = new int();
    int *num2 = new int;
    num = &a;
    if (*num == 1) {
        printf("pointer *num : %d", *num);
    }
    printf("*num : %d", *num);
    printf("&num : %s", &num);
}