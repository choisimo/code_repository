//
// Created by csimo on 10/7/2024.
//

/**
 * 세탁기 안에 있는 4개의 센서들
 * @Param 0 번 센서 : 세탁기 문이 닫혔는지 여부 (0 : 열림, 1: 닫힘)
 * @Param 1 번 센서 : 물이 차 있는지 여부 (0 : 없음, 1: 있음)
 * @Param 2 번 센서 : 세탁기 회전 중인지 여부 (0 : 회전 안 함, 1 : 회전 중)
 * @Param 3 번 센서 : 세탁물이 균형 잡혀 있는지 여부 (0 : 불균형, 1 : 균형)
 *
    2진수: 0b (C11 이상)
    8진수: 0
    10진수: 접두사 없음
    16진수: 0x
 */

#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS_

// P156 비트 연산 예제
int main(){
    int status = 0b0111; // 현재 상태 : 문 닫힘, 물 채워짐
    // 현재 세탁 중인가? (0 : 세탁 중 아님, 1 : 세탁 중)
    printf("문 열림 상태=%d\n", (status & 0b0100));
}