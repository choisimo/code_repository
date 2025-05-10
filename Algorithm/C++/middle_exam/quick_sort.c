#include <c++/tuple>
//
// Created by csimo on 24. 10. 28.
//
#include <stdio.h>

void swap(int *xp, int *yp);

/*int partition(int arr[], int p, int r){
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
}*/

int partition(int arr[], int p, int r)
{
    int pivot = arr[r];                  // 피벗을 배열의 마지막 요소로 선택
    int i = p - 1;                       // i 는 현재까지 피벗보다 작은 요소가 위치한 가장 오른쪽 인덱스

    for (int j = p; j < r; j++)
    {
        if (arr[j] < pivot)
        {
            i++;                          // 작은 요소를 찾았으므로 i를 증가
            swap(&arr[i], &arr[j]);  // arr[i] 와 arr[j] 를 교환하여 작은 요소를 왼쪽으로 이동
        }
    }
    swap(&arr[i + 1], &arr[r]);      // 피벗을 중앙 위치로 이동하여 분할 완료
    return i + 1;                         // 새로운 피벗 위치 반환
}

void swap (int *xp, int *yp)
{
    const int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void quickSort(int arr[], int p, int r)
{
    if (p < r)
    {
        int q = partition(arr, p, r);
        quickSort(arr, p, q - 1);
        quickSort(arr, q + 1, r);
    }
}