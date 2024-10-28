//
// Created by csimo on 24. 10. 28.
//
#include <stdio.h>

// n = 원소 개수, k = 현재 노드
void percolateDown(int arr[], int n, int k)
{
    int child = 2 * k + 1;
    int right = 2 * k + 2;
    int temp;
    int index = arr[0];

    // child 가 n 배열 내부에 있는지
    if (child < n)
    {
        if (arr[child] < arr[right] && right < n)
        {
            child = right;
        }

        index = arr[child];

    }
}