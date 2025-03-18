//
// Created by nodove on 24. 12. 2.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 45
void print(int lotto[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", lotto[i]);
    }
    printf("\n");
}

int main() {
    int lotto[6] = {0};
    int dup_check[45 + 1] = {0};
    int k;

    srand(time(NULL));
    for (int i = 0; i < 6; i++) {
        k = rand() % SIZE + 1;
        while (dup_check[k] == 1) {
            k = rand() % SIZE + 1;
        } lotto[i] = k; dup_check[k] = 1;
        printf("%d ", lotto[i]);
    }
}