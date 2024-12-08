//
// Created by nodove on 24. 12. 7.
//
#include <stdio.h>
#define MAX 100

// DP를 이용한 행렬 경로 문제 풀이
int matrixPath(int m[MAX][MAX], int n) {
    int c[MAX][MAX] = {0}; // DP를 위한 배열 초기화

    // 첫 번째 행 초기화
    for (int i = 0; i <=n; i++) {
        c[i][0] = 0;
    }

    // 첫 번째 열 초기화
    for (int j = 0; j <=n; j++) {
        c[0][j] = 0;
    }

    // DP를 이용한 행렬 경로 문제 풀이
    // 1부터 시작하는 이유 : 0부터 시작하면 인덱스가 0이 되어 오류 발생
    for (int i = 1; i <=n; i++) {
        for (int j = 1; j <= n; j++) {
            // 왼쪽 또는 위쪽에서 더 큰 값과 현재 값을 더함 (최대값 : 왼쪽 또는 위쪽에서 온 값 중 큰 값)
            c[i][j] = m[i][j] + ((c[i-1][j] > c[i][j-1]) ? c[i-1][j] : c[i][j-1]);
        }
    }
    return c[n][n];
}

int main() {
    int n;
    int m[MAX][MAX];

    // 행렬 크기 입력
    printf("행렬의 크기를 입력하세요 (n x n): ");
    scanf("%d", &n);

    // 행렬 값 입력
    printf("행렬의 값을 입력하세요:\n");
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            scanf("%d", &m[i][j]);
        }
    }

    // 결과 출력
    int maxScore = matrixPath(m, n);
    printf("최대 점수: %d\n", maxScore);

    return 0;
}
