//
// Created by nodove on 24. 11. 13.
//
#include <stdio.h>

int main() {
    int n;
    printf("정수를 입력하시오: ");
    scanf("%d", &n);

    int i = 0;
    while(i < n){
        int j = 0;
        while(j <= i) {
            printf("%d ", ++j);
        }
        ++i;
        printf("\n");
    }

}