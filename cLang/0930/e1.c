//
// Created by csimo on 9/30/2024.
//
#include <stdio.h>

int main(){
    int x, y, result;
    printf("두 수 입력 : ");
    scanf_s("%d %d", &x, &y);


    printf("%d + %d = %d\n", x, y, x + y);
    printf("%d - %d = %d\n", x, y, x - y);
    printf("%d * %d = %d\n", x, y, x * y);
    // 나눗셈
    printf("%d / %d = %d\n", x, y, x / y);
    // 나머지
    printf("%d %% %d = %d\n", x, y, x % y); // %% 써야한다!
    printf("%d %% %d = %d\n", x, y, (float)(x % y));
}
