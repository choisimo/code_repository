//
// Created by nodove on 24. 11. 13.
//
#include <stdio.h>

int main() {
    double fuel = 1000.0;                                   // 초기 연료량
    const double MAX_FUEL = 1000.0;                         // 최대 연료량
    const double WARNING_LEVEL = MAX_FUEL * 0.1;            // 경고 기준 (10%)
    double amount;
    char operation;

    printf("초기연료량: %.1f\n", fuel);

    while (1) {
        printf("연료 충전은 +, 소모는 -로 입력해주세요: ");
        scanf(" %c%lf", &operation, &amount);

        if (operation == '+') {
            fuel += amount;
            if (fuel > MAX_FUEL) {
                printf("최대 연료량을 초과했습니다. 현재 남아있는 양은 %.6f입니다. [최대 연료량 : 1000] \n", fuel);
                fuel = MAX_FUEL; // 최대 연료량을 초과할 경우 최대치로 설정
            }
        } else if (operation == '-') {
            if (fuel < amount){
                printf("연료가 부족합니다. 현재 남아있는 양은 %.6f입니다.\n", fuel);
                continue;
            }
            fuel -= amount;
            if (fuel < 0) {
                fuel = 0; // 연료가 0 미만이 되지 않도록 설정
            }
        } else {
            printf("잘못된 입력입니다. + 또는 -와 숫자를 입력해주세요.\n");
            continue;
        }

        printf("현재 남아있는 양은 %.6f입니다.\n", fuel);


        // 연료량이 10% 이하로 떨어지면 경고 출력 후 종료
        if (fuel <= WARNING_LEVEL) {
            printf("\n(경고) 연료가 10%% 이하입니다.\n");
            break;
        }
    }

    return 0;
}
