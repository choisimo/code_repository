//
// Created by csimo on 24. 10. 28.
//
#include <stdio.h>
#define BUCKET_SIZE 10  // bucket의 개수
#define MAX 100         // 정렬할 배열의 최대 크기
void insertSort(int arr[], int num);
void countingSort(double arr[], int n)
{
    int bucket_C[BUCKET_SIZE];
    int bucketOf[MAX];
    int B[MAX];

    // count 배열 0으로 초기화
    for (int i = 0; i < BUCKET_SIZE; i++)
    {
        bucket_C[i] = 0;
    }

    // 실수 버킷 인덱스 -> 정수 변환 후 count 배열에 삽입
    for (int i = 0; i < n; i++)
    {
        bucketOf[i] = arr[i] * BUCKET_SIZE;
        bucket_C[bucketOf[i]]++;
    }

    // 누적 합
    for (int i = 0; i < n; i++)
    {
        bucket_C[i+1] += bucket_C[i];
    }

    for (int i = n - 1; i >= 0; i--)
    {
        int bucket_index = bucketOf[i];
        B[--bucket_C[bucket_index]] = arr[i];
    }

    insertSort(B, n);
}

void insertSort(int B[], int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = B[i];
        int j = i - 1;
        for (; j >= 0 && B[j] > key; j--)
        {
            B[j + 1] = B[j];
        }
        B[j + 1] = key;
    }
}