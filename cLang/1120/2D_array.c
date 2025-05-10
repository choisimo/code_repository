//
// Created by nodove on 24. 11. 20.
//
#include <stdio.h>

int main() {
    int c1[3][3] = {
            {2401, 90, 80}, {2402, 78, 90}, {2403, 79, 75}
    };
    float sum = 0.0, avg = 0.0;

    for (int i = 0; i < 3; i++) {
        for (int k = 1; k < 3; k++) {
            sum += c1[i][k];
        }
        printf("| 학번 : %d | ", c1[i][0]);
        printf("총 점수 : %.2f \t", sum);
        printf("average : %.1f", sum / 2);
        sum = 0.0;    printf("\n");

    }
    printf("\n");
}

