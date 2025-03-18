//
// Created by nodove on 24. 11. 13.
//
#include <stdio.h>

int main() {
    int cnt = 10;
    while (cnt != 0) {
        int n = 0;

        printf("데이터를 입력하시오: ");
        scanf("%d", &n);
        if (n <= 0 || n > 50) {
            printf("1 이상 50 이하의 정수를 입력하세요.\n");
            continue;
        }
        cnt--;
        while (n >= 1) {
            n--;
            printf("*");
        }
        printf(" [남은 횟수 : %d]\n", cnt);
    }

}