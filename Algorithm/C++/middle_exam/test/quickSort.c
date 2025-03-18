//
// Created by csimo on 24. 10. 28.
//
#include <stdio.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int p, int r)
{
    int pivot = arr[r];
    int i = p - 1;

    for (int j = p; j < r; j++)
    {
        if (arr[j]< pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[r]);

    return i + 1;
}

void quickSort(int arr[], int p, int r)
{
    if (p < r)
    {
        int q = partition(arr, p, r);
        quickSort(arr, p , q - 1);
        quickSort(arr, q + 1, r);
    }
}