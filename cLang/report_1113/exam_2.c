//
// Created by nodove on 24. 11. 13.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
    int n = 0;
    printf("몇번이나 반복할까요?");
    scanf("%d", &n);
    if (n <= 0){
        return 0;
    }
    for (int i = 0; i < n; i++) {
        printf("안녕하세요\n");
    }
}