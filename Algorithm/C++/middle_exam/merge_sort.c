#include <float.h>
//
// Created by csimo on 10/27/2024.
//
void merge(int list[], int p, int q, int r)
{
    int i = p, j = q + 1, t = 0;
    int temp[r - p + 1];

    while (i <= q && j <= r){
        if (list[i] <= list[j]){
            temp[t++] = list[i++];
        } else
        {
            temp[t++] = list[j++];
        }
    }

    while (i <= q)
    {
        temp[t++] = list[i++];
    }

    while (j <= r)
    {
        temp[t++] = list[j++];
    }

    for (i = p, t = 0; i <= r; i++, t++)
    {
        list[i] = temp[t];
    }
}

void mergeSort(int list[], int p, int r){
    int q = (p + r) / 2;
    if (p < r) {
        mergeSort(list, p, q);
        mergeSort(list, q + 1, r);
        merge(list, p, q, r);
    }
}

