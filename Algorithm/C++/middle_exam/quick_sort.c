#include <c++/tuple>
//
// Created by csimo on 24. 10. 28.
//
#include <stdio.h>

void swap(int *xp, int *yp);

int partition(int arr[], int p, int r){
    int x = arr[r];
    int i = p - 1;

    for (int j = p; j < r; j++)
    {
        if (arr[j] < x)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[r]);
    return i + 1;
}

void swap (int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}