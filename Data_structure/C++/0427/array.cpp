#include <iostream>
#include <cstdlib>
//
// Created by csimo on 4/27/2024.
//
int main(){
    int* arr = new int[5];
    int* arr_malloc = (int*)malloc(sizeof(int) * 5);
    delete[] arr;
    free(arr_malloc);
    int* origin = (int*)malloc(10 * sizeof(int));
    for(int i = 0; i < 10; i++) origin[i] = i;
    int* change = (int*)malloc(100*sizeof(int));
    for(int i = 0; i < 10; i++) change[i] = origin[i];
    free(origin);origin=change;free(change);
}
