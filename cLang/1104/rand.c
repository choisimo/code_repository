//
// Created by nodove on 24. 11. 4.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int answer, guess, tries = 0;
    srand(time(NULL)); // 현재 시간을 이용하여 seed값을 생성
    answer = rand() % 100; // 0~99 사이의 난수 생성
    printf("answer: %d\n", answer); // 정답 출력

    do
    {
        printf("정답 입력 : ");                             // 사용자 입력 요청
        scanf("%d", &guess);                               //  사용자 입력
        tries++;                                                // 시도 횟수 증가
        if (guess > answer)                                      // 입력값이 정답보다 큰 경우
            printf("더 작은 수를 입력하세요.\n");              // 사용자에게 힌트 제공
        else if (guess < answer)                                 // 입력값이 정답보다 작은 경우
            printf("더 큰 수를 입력하세요.\n");                // 사용자에게 힌트 제공
        else
            printf("정답입니다. 시도 횟수 : %d\n", tries);
    } while (guess != answer);

    printf("정답 : %d, 시도 횟수 : %d\n", answer, tries);
    return 0;
}