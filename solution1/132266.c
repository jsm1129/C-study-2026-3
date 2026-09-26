#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// roads_rows는 2차원 배열 roads의 행 길이, roads_cols는 2차원 배열 roads의 열 길이입니다.
// sources_len은 배열 sources의 길이입니다.
int* solution(int n, int** roads, size_t roads_rows, size_t roads_cols, int sources[], size_t sources_len, int destination) {
    // ---- 1. 인접 리스트 구성 (CSR 방식: degree -> offset -> adjacency) ----
    // 지역 번호가 1~n이므로 배열도 n+1 크기로 잡음 (0번 인덱스는 안 씀)
    int *degree = (int*)calloc(n + 1, sizeof(int));   // 지역별 연결 개수(차수) 저장
    for (size_t i = 0; i < roads_rows; i++) {
        int a = roads[i][0];
        int b = roads[i][1];
        degree[a]++;   // a쪽 연결 개수 +1
        degree[b]++;   // 양방향 도로이므로 b쪽도 +1
    }

    // offset[i] ~ offset[i+1]-1 구간이 지역 i의 인접 노드들이 저장될 위치
    int *offset = (int*)malloc((n + 2) * sizeof(int));  // 각 지역 인접리스트 시작 위치(누적합)
    offset[1] = 0;
    for (int i = 1; i <= n; i++) {
        offset[i + 1] = offset[i] + degree[i];  // 차수를 누적해서 시작 인덱스 계산
    }

    int total_edges = offset[n + 1];              // 전체 간선 저장 공간 (양방향이라 roads_rows*2)
    int *adj = (int*)malloc(total_edges * sizeof(int));  // 실제 인접 노드 번호들을 일렬로 저장
    int *cursor = (int*)malloc((n + 1) * sizeof(int));   // 각 지역마다 다음에 채울 위치 추적용
    for (int i = 1; i <= n; i++) cursor[i] = offset[i];  // 커서를 각자의 시작 위치로 초기화

    // roads를 순회하며 adj 배열에 실제로 양방향 연결 정보를 채워 넣음
    for (size_t i = 0; i < roads_rows; i++) {
        int a = roads[i][0];
        int b = roads[i][1];
        adj[cursor[a]++] = b;  // a의 인접리스트에 b 추가 후 커서 한 칸 이동
        adj[cursor[b]++] = a;  // b의 인접리스트에 a 추가 후 커서 한 칸 이동
    }

    // ---- 2. destination에서 BFS로 최단거리 계산 ----
    int *dist = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) dist[i] = -1;  // -1: 미방문(=아직 도달 불가로 간주) 초기화

    int *queue = (int*)malloc((n + 1) * sizeof(int));  // 배열로 구현한 큐 (n개면 충분)
    int head = 0, tail = 0;  // head: 꺼낼 위치, tail: 넣을 위치

    dist[destination] = 0;      // 시작점은 거리 0
    queue[tail++] = destination; // 큐에 시작점 삽입

    while (head < tail) {                 // 큐가 빌 때까지 반복
        int u = queue[head++];            // 큐에서 하나 꺼냄
        for (int idx = offset[u]; idx < offset[u + 1]; idx++) {  // u의 인접 노드들 순회
            int v = adj[idx];
            if (dist[v] == -1) {           // 아직 방문 안 한 노드만 처리 (중복 방문 방지)
                dist[v] = dist[u] + 1;      // 한 단계 더 간 거리로 갱신
                queue[tail++] = v;          // 다음에 탐색할 큐에 삽입
            }
        }
    }

    // ---- 3. sources 순서대로 결과 배열 채우기 ----
    int *answer = (int*)malloc(sources_len * sizeof(int));
    for (size_t i = 0; i < sources_len; i++) {
        answer[i] = dist[sources[i]];  // BFS로 미리 구해둔 거리값을 그대로 꺼내 씀 (도달 불가면 -1 그대로)
    }

    // ---- 4. 임시로 썼던 메모리 정리 (answer는 반환해야 하므로 free하지 않음) ----
    free(degree);
    free(offset);
    free(adj);
    free(cursor);
    free(dist);
    free(queue);

    return answer;
}