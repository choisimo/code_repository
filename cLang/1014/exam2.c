//
// Created by nodove on 24. 10. 14.
//
#include <stdio.h>

int main() {
    int input;
    scanf_s("%d", &input);

    if (input % 2 == 0) {
        printf("%d �� ¦���Դϴ�.", input);
    } else {
        printf("%d �� Ȧ���Դϴ�.", input);
    }
}
