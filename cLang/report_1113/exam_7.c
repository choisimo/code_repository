//
// Created by nodove on 24. 11. 13.
//
#include <stdio.h>


int main() {
    int x, y;

    printf("시작 정수: ");
    scanf("%d", &x);

    printf("종료 정수: ");
    scanf("%d", &y);

    int sum = 0;

    if (x > y) {
        for (int i = y; i <= x; i++) {
            sum += i;
        }
    } else {
        for (int i = x; i <= y; i++) {
            sum += i;
        }
    }

    printf("%d에서 %d까지의 합: %d\n", x, y, sum);

    return 0;
}
