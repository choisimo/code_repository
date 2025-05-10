//
// Created by nodove on 24. 12. 7.
//
void bfs(int v) {
    int front = 0, rear = 0;                   // 큐 초기화
    visit[v] = true;                    // 시작 정점 방문 표시
    printf("%d", v);                    // 시작 정점 출력
    addq(v);                            // 큐에 시작 정점 추가

    while (front) {                     // 큐가 비어 있지 않을 동안
        v = deleteq();                  // 큐에서 정점 제거
        for (w = graph[v]; w; w = w->link) { // 현재 정점의 인접 정점 탐색
            if (!visit[w->ver]) {       // 방문하지 않은 정점 탐색
                visit[w->ver] = true;   // 방문 처리
                printf("%d", w->ver);   // 방문 정점 출력
                addq(w->ver);           // 큐에 추가
            }
        }
    }
}
