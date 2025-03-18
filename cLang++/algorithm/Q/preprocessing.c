//
// Created by nodove on 24. 12. 5.
//

#include <stdio.h>

int m = 7;

void preprocessing(const char *P, int *fail, int m) {
    fail[0] = -1;                           // fail[0] = -1 고정
    for (int j = 1; j < m; j++) {           // j = 1 ~ 6
        int i = fail[j - 1];                    // i = fail[0] ~ fail[5]
        while(i >= 0 && P[i + 1] != P[j]) { // i = 0 ~ 5, P[i + 1] != P[j]일 때
            i = fail[i];
        }
        if (P[j] == P[i + 1]) fail[j] = i + 1;
        else fail[j] = -1;
    }
}

int main() {
    char* P = "abcabcac";
    printf("before preprocessing\n");
    for (int i = 0; i < m; i++) {
        printf("%c ", P[i]);
    }
    int fail[m];
    preprocessing(P, fail, m);
    printf("\nafter preprocessing\n");
    for (int i = 0; i < m; i++) {
        printf("%d ", fail[i]);
    }
}
