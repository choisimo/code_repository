다음은 다익스트라 알고리즘의 빈칸 문제입니다. 이 문제를 통해 코드를 암기하는 데 도움을 받을 수 있을 것입니다.

---

### 다익스트라 알고리즘 빈칸 문제

다음 코드는 다익스트라 알고리즘을 구현한 것입니다. 빈칸(______)을 채워 완성하세요.

---

```c
Dijkstra(v, cost[][], dist[], n, found[]) {
    for (i = 0; i < n; i++) {
        found[i] = ________;  // 초기 모든 정점 방문 여부
        dist[i] = ________;  // 시작 정점 v에서 각 정점까지의 초기 거리
    }
    found[v] = ________;  // 시작 정점 방문 처리

    for (i = 0; i < ________; i++) {  // n-2번 반복
        u = ________(dist, n, found);  // 최소 거리 정점 선택
        found[u] = ________;  // 선택된 정점 방문 처리

        for (w = 0; w < ________; w++) {  // 모든 정점 확인
            if (!found[w] && dist[w] > dist[u] + cost[u][w]) {
                dist[w] = ________;  // 최소 거리로 갱신
            }
        }
    }
}
```

```c
extractMin(dist[], n, found[]) {
    min = ________;
    minpos = ________;

    for (i = 0; i < n; i++) {
        if (!found[i] && dist[i] < ________) {  // 방문하지 않은 정점 중 최소 거리
            min = ________;
            minpos = ________;
        }
    }
    return ________;  // 최소 거리 정점 반환
}
```

---

### 정답
#### `Dijkstra` 함수
1. `false`
2. `cost[v][i]`
3. `true`
4. `n - 2`
5. `extractMin`
6. `true`
7. `n`
8. `dist[u] + cost[u][w]`

#### `extractMin` 함수
1. `MAX`
2. `-1`
3. `min`
4. `dist[i]`
5. `i`
6. `minpos`

---

이 문제를 반복해서 풀다 보면 코드를 빠르게 암기할 수 있을 것입니다. 😊