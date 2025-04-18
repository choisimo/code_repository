
### `partition` 함수의 코드
```c
int partition(int arr[], int p, int r) {
    int x = arr[r]; // 피벗을 배열의 마지막 요소로 선택
    int i = p - 1;

    for (int j = p; j < r; j++) {
        if (arr[j] < x) {
            i++;
            swap(&arr[i], &arr[j]); // 피벗보다 작은 값을 왼쪽에 배치
        }
    }
    swap(&arr[i + 1], &arr[r]); // 피벗을 올바른 위치에 배치
    return i + 1; // 피벗의 최종 위치 반환
}
```

### 코드 설명 및 동작 과정

1. **피벗 선택**: 
   ```c
   int x = arr[r]; // 피벗을 배열의 마지막 요소로 선택
   ```
   - `arr[r]`(배열의 마지막 요소)을 **피벗 값**으로 선택합니다. 이 값 `x`를 기준으로 배열을 나눌 것입니다.
   - 예를 들어, `arr = [31, 8, 48, 73, 11, 3, 20, 29, 65, 15]`에서 `r = 9`일 때, `x = arr[9] = 15`가 됩니다.

2. **초기 인덱스 설정**:
   ```c
   int i = p - 1;
   ```
   - `i`는 피벗보다 작은 값들을 배치할 위치를 나타내는 인덱스로, `p - 1`로 초기화됩니다.
   - 여기서 `p`는 배열의 시작 인덱스입니다. `i`를 `p - 1`로 설정하는 이유는, **피벗보다 작은 값이 발견될 때마다 `i`를 1 증가시키며 `i`가 가리키는 위치에 작은 값을 채우기 위해서**입니다.
   - 예를 들어, `p = 0`일 경우 `i`는 `-1`부터 시작합니다.

3. **반복문을 통한 피벗 기준 분할**:
   ```c
   for (int j = p; j < r; j++) {
       if (arr[j] < x) {
           i++;
           swap(&arr[i], &arr[j]); // 피벗보다 작은 값을 왼쪽에 배치
       }
   }
   ```
   - `j`는 `p`부터 `r - 1`까지 순회하며, `arr[j]`와 `x`(피벗)를 비교합니다.
   - `if (arr[j] < x)` 조건문으로, `arr[j]`가 피벗보다 작으면 **작은 값을 `i`의 다음 위치에 배치**합니다.
     - `i++`로 `i`를 1 증가시킨 뒤, `arr[i]`와 `arr[j]`를 교환하여 **작은 값들을 왼쪽부터 차례로 채워나갑니다**.

#### 예시를 통한 이해
배열: `[31, 8, 48, 73, 11, 3, 20, 29, 65, 15]`, `p = 0`, `r = 9`, `x = 15`
- 초기 상태: `i = -1`

| `j` | `arr[j]` | `arr[j] < x` | `i` | `i++` 후 `arr[i]`와 `arr[j]` 교환 결과        |
|-----|----------|--------------|-----|---------------------------------------------|
| 0   | 31       | 아니요       | -1  | (변화 없음)                                 |
| 1   | 8        | 예           | 0   | `[8, 31, 48, 73, 11, 3, 20, 29, 65, 15]`    |
| 2   | 48       | 아니요       | 0   | (변화 없음)                                 |
| 3   | 73       | 아니요       | 0   | (변화 없음)                                 |
| 4   | 11       | 예           | 1   | `[8, 11, 48, 73, 31, 3, 20, 29, 65, 15]`    |
| 5   | 3        | 예           | 2   | `[8, 11, 3, 73, 31, 48, 20, 29, 65, 15]`    |
| 6   | 20       | 아니요       | 2   | (변화 없음)                                 |
| 7   | 29       | 아니요       | 2   | (변화 없음)                                 |
| 8   | 65       | 아니요       | 2   | (변화 없음)                                 |

- 반복문이 종료되면, `i = 2`가 됩니다. 현재 배열 상태는 `[8, 11, 3, 73, 31, 48, 20, 29, 65, 15]`입니다.

4. **피벗을 최종 위치로 이동**:
   ```c
   swap(&arr[i + 1], &arr[r]); // 피벗을 올바른 위치에 배치
   return i + 1; // 피벗의 최종 위치 반환
   ```
   - 마지막으로 `arr[i + 1]`과 `arr[r]`을 교환하여 **피벗을 `i + 1` 위치에 둡니다**.
   - 이로써 피벗 `15`의 왼쪽에는 모두 `15`보다 작은 값들이, 오른쪽에는 큰 값들이 배치됩니다.
   - 이 예시에서, `i + 1 = 3`이므로 `arr[3]`과 `arr[9]`을 교환하여 배열은 `[8, 11, 3, 15, 31, 48, 20, 29, 65, 73]`이 됩니다.

### 최종 반환값
- `partition` 함수는 **피벗의 최종 위치**인 `i + 1`을 반환합니다. 퀵 정렬의 다음 재귀 호출에서는 이 반환값을 기준으로 **왼쪽 부분 배열**과 **오른쪽 부분 배열**을 각각 정렬합니다.

### 전체 요약
- `int i = p - 1;`는 작은 값들을 배치할 인덱스를 설정하기 위한 초기값입니다.
- `for` 루프에서 `arr[j] < x` 조건을 만족하는 경우, `i++`를 통해 작은 값들을 왼쪽으로 순차적으로 배치합니다.
- 마지막에 `arr[i + 1]`과 피벗 `arr[r]`을 교환하여 피벗을 올바른 위치에 둡니다.

이 방식으로 배열이 피벗을 기준으로 분할되며, `partition` 함수는 피벗의 최종 위치를 반환합니다.


`partition` 함수에서 **swap**을 수행하는 이유는 **피벗 값을 올바른 위치에 배치**하여, 피벗을 기준으로 배열을 두 부분으로 나누기 위해서입니다. 

### 왜 `swap(&arr[i + 1], &arr[r])`이 필요한가?

퀵 정렬의 `partition` 함수는 배열을 피벗 값 기준으로 두 부분으로 나누는 역할을 합니다. 피벗을 기준으로 나누기 위해 다음과 같은 조건이 필요합니다:
- 피벗보다 **작은 값들은 피벗의 왼쪽**에 있어야 하고,
- 피벗보다 **큰 값들은 피벗의 오른쪽**에 있어야 합니다.

#### `swap`의 필요성 설명

1. **피벗의 위치 선정**:
   - `partition` 함수에서는 배열의 마지막 요소 `arr[r]`을 피벗 값으로 선택합니다 (`x = arr[r]`).
   - 함수가 끝날 때 피벗은 정렬된 배열에서 제 위치에 있어야 합니다.

2. **분할 과정**:
   - `for` 루프를 통해 배열을 순회하면서, 피벗보다 작은 값은 왼쪽으로, 큰 값은 오른쪽으로 정렬합니다.
   - 이때 `i`는 피벗보다 작은 값이 배치될 최종 위치를 가리키고 있습니다.

3. **피벗을 제 위치에 배치**:
   - `for` 루프가 끝나고 나면, `i`는 **피벗보다 작은 값들의 끝 위치**를 가리킵니다.
   - 따라서, **피벗을 `i + 1` 위치에 놓아야** 피벗보다 작은 값들은 왼쪽에, 큰 값들은 오른쪽에 있게 됩니다.
   - 이때 `swap(&arr[i + 1], &arr[r])`을 수행하여, `arr[r]`에 있던 피벗을 `arr[i + 1]`로 이동시킵니다.

4. **결과**:
   - `swap`을 통해 피벗이 제 위치로 이동하면서, 배열은 피벗을 기준으로 두 부분으로 분할됩니다.
   - 최종적으로 `partition` 함수는 피벗의 최종 위치인 `i + 1`을 반환합니다. 이후 `quickSort`가 이 위치를 기준으로 왼쪽과 오른쪽 부분을 각각 재귀적으로 정렬합니다.

### 예시로 설명
예를 들어, 배열이 `[31, 8, 48, 73, 11, 3, 20, 29, 65, 15]`이고, `15`가 피벗이라면:
1. `partition` 함수가 끝나기 전 배열 상태는 `[8, 11, 3, 73, 31, 48, 20, 29, 65, 15]`이고, `i = 2`입니다.
2. `swap(&arr[i + 1], &arr[r])`을 수행하여 `arr[3]`과 `arr[9]`(피벗)을 교환합니다.
3. 교환 후 배열은 `[8, 11, 3, 15, 31, 48, 20, 29, 65, 73]`가 되고, 피벗 `15`는 정렬된 위치에 배치됩니다.

### 요약
`swap`은 피벗을 올바른 위치에 배치하여, 피벗을 기준으로 배열을 두 부분으로 나누기 위해 필수적입니다. 이를 통해 퀵 정렬이 피벗을 중심으로 재귀적으로 정렬을 진행할 수 있습니다.