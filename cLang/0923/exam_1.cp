//
// Created by csimo on 9/23/2024.
//
#include <stdio.h>

int main(){
    long long int property;
    long long int saving;

    printf("�Ŵ� ���� �ݾ� �Է� : ");
    scanf_s("%lld", &saving);

    property = saving * 12 * 30;
    printf("30�� ���� ��� : %lld\n", property);

}