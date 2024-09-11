//
// Created by csimo on 9/9/2024.
//
#include <stdio.h>

// 두 개의 정수를 저장할 구조체 정의
struct Numbers {
    int num1;
    int num2;
};

// 두 수의 합을 계산하는 함수
int sum(struct Numbers n) {
    return n.num1 + n.num2;
}

int main() {
    struct Numbers nums;  // 구조체 변수 선언

    // 사용자로부터 두 수 입력 받기
    printf("첫 번째 숫자를 입력하세요: ");
    scanf("%d", &nums.num1);

    printf("두 번째 숫자를 입력하세요: ");
    scanf("%d", &nums.num2);

    // 합 계산 및 출력
    int result = sum(nums);
    printf("두 수의 합: %d\n", result);

    return 0;
}
