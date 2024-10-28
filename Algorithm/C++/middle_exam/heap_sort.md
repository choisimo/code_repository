주어진 `percolateDown` 알고리즘을 재귀 방식으로 구현한 C 코드는 아래와 같습니다. 이 코드는 현재 노드와 자식 노드를 비교하면서 힙 속성을 유지하기 위해 재귀적으로 호출합니다.

### 재귀 방식으로 구현된 `percolateDown`

```c
#include <stdio.h>

void percolateDown(int A[], int n, int k) {
    int child = 2 * k + 1; // 왼쪽 자식 노드
    int right = 2 * k + 2; // 오른쪽 자식 노드

    // 자식 노드가 배열 범위 내에 있는지 확인
    if (child >= n) return;

    // 오른쪽 자식이 존재하고, 오른쪽 자식이 왼쪽 자식보다 크다면 child를 오른쪽으로 설정
    if (right < n && A[child] < A[right]) {
        child = right;
    }

    // 현재 노드가 자식 노드보다 작다면 교환하고, 재귀적으로 percolateDown을 호출
    if (A[k] < A[child]) {
        int temp = A[k];
        A[k] = A[child];
        A[child] = temp;

        // 재귀적으로 자식 노드에 대해 percolateDown 호출
        percolateDown(A, n, child);
    }
}

// 디버깅을 위해 배열을 출력하는 함수
void printArray(int A[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

// 테스트 코드
int main() {
    int A[] = {4, 7, 3, 2, 1, 8, 9, 5, 6};
    int n = sizeof(A) / sizeof(A[0]);
    int k = 0; // 시작 노드 인덱스

    percolateDown(A, n, k);
    printArray(A, n);

    return 0;
}
```

### 코드 설명

1. **초기 설정**:
   - `child`는 `2 * k + 1`로 왼쪽 자식 노드의 인덱스를, `right`는 `2 * k + 2`로 오른쪽 자식 노드의 인덱스를 가집니다.
   
2. **자식 노드 범위 확인**:
   - `child >= n`이면 자식 노드가 없는 경우로, 함수가 종료됩니다.

3. **오른쪽 자식 선택**:
   - `right < n` 조건을 통해 오른쪽 자식이 있는지 확인하고, 오른쪽 자식 값이 더 크면 `child`를 오른쪽 자식으로 변경합니다.
   
4. **현재 노드와 자식 노드 비교 및 교환**:
   - `A[k] < A[child]` 조건이 참이면, 현재 노드와 자식 노드의 값을 교환합니다.
   - 교환 후, `percolateDown`을 자식 노드에 대해 재귀적으로 호출하여 힙 조건을 만족하도록 합니다.

5. **테스트 코드**:
   - `main` 함수에서 배열을 정의하고 `percolateDown`을 호출하여 첫 번째 인덱스에서 시작하는 힙 구조를 만듭니다.
   - 이후 `printArray` 함수로 정렬된 배열을 출력하여 결과를 확인합니다.

이 재귀 방식은 특정 노드에서 시작해 힙 구조를 유지하는 데 효과적이며, 재귀 호출을 통해 하위 레벨까지 힙 조건을 만족하도록 합니다.

