//
// Created by csimo on 10/2/2024.
//
#include <stdio.h>

int main(){
    // p144
    int x = 9, y = 10;
    // x = 1001, y = 1010
    printf("%d & %d\n", x, y, x & y); //8
    printf("%d | %d\n", x, y, x | y); //11
    printf("%d ^ %d\n", x, y, x ^ y); //3
    printf("~%d = %d\n", x, ~x + 1); //9 음수 -> ~는 1의 보수이다!!(중요)

    /*
     * 비트 NOT 연산자
     * 부호 절대값 :
     * 1의 보수 :
     * 2의 보수 :
     * */
}