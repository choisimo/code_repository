//
// Created by nodove on 24. 11. 13.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>


int main() {
    int input = 0, sum = 0;

    while (1) {
        printf("정수를 입력하시오 (0 입력 시 종료): ");
        scanf("%d", &input);

        if (input == 0) {
            break; // 입력이 0이면 루프 종료
        }

        sum += input;
    }

    printf("합계: %d\n", sum);
    return 0;
}
