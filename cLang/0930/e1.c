//
// Created by csimo on 9/30/2024.
//
#include <stdio.h>

int main(){
    int x, y, result;
    printf("�� �� �Է� : ");
    scanf_s("%d %d", &x, &y);


    printf("%d + %d = %d\n", x, y, x + y);
    printf("%d - %d = %d\n", x, y, x - y);
    printf("%d * %d = %d\n", x, y, x * y);
    // ������
    printf("%d / %d = %d\n", x, y, x / y);
    // ������
    printf("%d %% %d = %d\n", x, y, x % y); // %% ����Ѵ�!
    printf("%d %% %d = %d\n", x, y, (float)(x % y));
}
