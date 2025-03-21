### 돌 놓기 문제 (DP) 빈칸 문제

다음은 돌 놓기 문제에 대한 코드입니다. 빈칸(______)을 채워 코드를 완성하세요.

---

```c
pebble(n) {
    // 1단계: 첫 번째 열 초기화
    for (p = 1 to 4) { 
        peb[1][p] = ________;  // 첫 번째 열에서 p 패턴 점수
    }
    // 2단계: 동적 프로그래밍 계산
    for (i = 2 to n) {  // 두 번째 열부터 시작
        for (p = 1 to 4) {  // 현재 열의 각 패턴에 대해
            peb[i][p] = max(__________) + _________;  // 이전 열의 최적 값 + 현재 점수
        }
    }
    // 3단계: 결과 반환
    return max(__________);  // 마지막 열에서 최대값 반환
}
```

---

### 문제 조건
1. `w[p][i]`는 열 `i`의 패턴 `p`에 해당하는 점수를 의미합니다.
2. 패턴 `q`는 패턴 `p`와 양립 가능한 패턴입니다.

---

### 풀이 힌트
- 첫 번째 열에서는 단순히 해당 패턴 점수를 넣으면 됩니다.
- 두 번째 열부터는 이전 열의 가능한 패턴 중 최적 점수를 선택하고 현재 패턴 점수를 더합니다.
- 마지막 열에서 최댓값을 반환합니다.

---

### 정답 (빈칸 해답)
1. `w[p][1]`
2. `peb[q][i-1]` (q는 패턴 p와 양립 가능한 패턴)
3. `w[p][i]`
4. `peb[1][n], peb[2][n], peb[3][n], peb[4][n]`

---

이 문제를 연습하여 패턴별 최적 점수 계산의 과정을 명확히 이해하세요! 😊