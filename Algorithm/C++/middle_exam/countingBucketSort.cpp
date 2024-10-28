//
// Created by csimo on 24. 10. 28.
//
#include <stdio.h>
#define MAX 100
#define BUCKET_SIZE 10

void insertionSort(int arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;
        for (; i >= 0 && key < arr[j]; j--)
        {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = key;
    }
}

void countingBucketSort(double arr[], int n)
{
    int C[BUCKET_SIZE];
    int bucketOf[MAX];
    int B[MAX];

    // 카운트 배열 초기화
    for (int i = 0; i < BUCKET_SIZE; i++)
    {
        C[i] = 0;
    }

    // 버킷 할당 및 카운트 증가
    for (int i = 0; i < n; i++)
    {
        bucketOf[i] = arr[i] * BUCKET_SIZE;
        C[bucketOf[i]]++;
    }

    // 누적합 계산
    for (int i = 0; i < BUCKET_SIZE; i++)
    {
        C[i] += C[i - 1];
    }

    // 정렬된 위치에 원소 배치
    for (int i = n - 1; i >= 0; i--)
    {
        int bucketIndex = bucketOf[i];
        B[--C[bucketIndex]] = arr[i] * 100;
    }

    insertionSort(B, BUCKET_SIZE);
}