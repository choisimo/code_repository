//
// Created by csimo on 9/23/2024.
//
#include <stdio.h>

int main(){
    long long int property;
    long long int saving;

    printf("매달 적금 금액 입력 : ");
    scanf_s("%lld", &saving);

    property = saving * 12 * 30;
    printf("30년 후의 재산 : %lld\n", property);

}