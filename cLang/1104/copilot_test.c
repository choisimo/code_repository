//
// Created by nodove on 24. 11. 3.
//
#include <stdio.h>

int main() {
    int i = 0;
    do
    {
        printf("1--파일 열기\n");
        printf("2--파일 저장\n");
        printf("3--파일 닫기\n");
        printf("4--종료\n");
        printf("번호를 선택하세요: ");
        scanf("%d", &i);
    } while (i < 1 || i > 4);
    printf("선택된 번호는 %d입니다.\n", i);
    return 0;
}   // end of main