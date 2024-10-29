//
// Created by csimo on 24. 10. 28.
//
#include <stdio.h>

// 최대 힙을 유지하기 위해 특정 노드를 아래로 내려가면서 자식 노드들과 비교하여 위치 조정
void percolateDown(int A[], int n, int k)
{
    int child = 2 * k + 1;
    int right = 2 * k + 2;
    int temp;

    if (child < n)
    {
        // child 는 더 큰 값을 가진 노드 선택
        if (right < n && A[child] < A[right])
        {
            child = right;
        }

        // 현재 노드와 선택된 자식 노드를 비교하여 힙 성질을 유지하기 위해 교환이 필요한지 확인
        if (A[k] < A[child])
        {
            temp = A[k];
            A[k] = A[child];
            A[child] = temp;

            percolateDown(A, child, k);
        }
    }
}


int deleteMax(int A[], int *n)
{
    int max = A[0];
    A[0] = A[*n - 1];
    (*n)--;

    percolateDown(A, *n, 0);

    return max;
}

void buildHeap(int A[], int n)
{
    // 마지막 비단말 노드 (자식을 가지는) n - 2 / 2
    for (int i = (n - 2) / 2; i >= 0; i--) {
        // 각 비단말 노드에서 percolateDown 호출하여 힙 성질 유지
        percolateDown(A, n, i);
    }
}

void heapSort(int A[], int n)
{
    // 최대 힙 생성
    buildHeap(A, n);

    for (int i = n - 1; i > 0; i--)
    {
        int temp = A[0];
        A[0] = A[i];
        A[i] = temp;

        percolateDown(A, i, 0);
    }

}
