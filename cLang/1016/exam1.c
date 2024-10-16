//
// Created by csimo on 10/16/2024.
//
#include<stdio.h>

int main() {
    int var;
    printf("정수 입력 : ");
    scanf_s("%d", &var);

    switch(var) {
        case 0 :
            printf("없음");
            break;
        case 1 :
            printf("하나");
            break;
        case 2 :
            printf("둘");
            break;
    }
}