//
// Created by csimo on 24. 10. 29.
//

#include <stdio.h>
void merge(int a[], int a1, int a2, int a3);

void mergeSort(int arr[], int p, int r)
{
    if (p < r)
    {
        int q = (p + r) / 2;
        mergeSort(arr, p, q);
        mergeSort(arr, q + 1, r);
        merge(arr, p, q, r);
    }
}

void merge(int arr[], int p, int q, int r)
{
    int temp[r - p + 1];

    int i = p; int j = q + 1; int t = 0;
    while( i <= q && j <= r)
    {
        if (arr[i] <= arr[j])
        {
            temp[t++] = arr[i++];
        } else
        {
            temp[t++] = arr[j++];
        }
    }

    while (i <= q)
    {
        temp[t++] = arr[i++];
    }

    while (j <= r)
    {
        temp[t++] = arr[j++];
    }

    for (i = p, t = 0; i <= r;)
    {
        arr[i++] = temp[t++];

    }

}
