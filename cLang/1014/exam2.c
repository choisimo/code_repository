//
// Created by nodove on 24. 10. 14.
//
#include <stdio.h>

int main() {
    int input;
    scanf_s("%d", &input);

    if (input % 2 == 0) {
        printf("%d 는 짝수입니다.", input);
    } else {
        printf("%d 는 홀수입니다.", input);
    }
}
