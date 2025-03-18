//
// Created by csimo on 24. 10. 28.
//
#include <stdio.h>

void percolateDown(int arr[], int n, int k)
{
    int child = 2 * k + 1;
    int right = 2 * k + 2;
    int temp;

    if (child < n)
    {
        if (right < n && arr[child] < arr[right])
        {
            child = right;
        }

        if (arr[k] < arr[child])
        {
            temp = arr[k];
            arr[k] = arr[child];
            arr[child] = temp;

            percolateDown(arr, n, child);

        }
    }
}

int DeleteMaxHeap(int arr[], int *n)
{
    int max = arr[0];
    arr[0] = arr[*n - 1];
    (*n)--;

    percolateDown(arr, *n, 0);

    return max;
}
