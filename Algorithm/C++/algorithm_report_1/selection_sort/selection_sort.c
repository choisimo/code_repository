//
// Created by csimo on 10/4/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define s_num 916
#define _CRT_SECURE_NO_WARNINGS_


int main(){
    int size = 1000;
    int* arr = (int *) malloc(size * sizeof(int));
    int seed = s_num % 1000;

    srand(seed);

    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
    }


    // allocated memory set free
    free(arr);
}