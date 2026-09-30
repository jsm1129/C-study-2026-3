#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// lighthouse_rows는 2차원 배열 lighthouse의 행 길이, lighthouse_cols는 2차원 배열 lighthouse의 열 길이입니다.
int solution(int n, int** lighthouse, size_t lighthouse_rows, size_t lighthouse_cols) {
    int answer = 0;
    int m = (int)lighthouse_rows; // 간선 수 (= n - 1)

    // CSR 방식 인접 리스트: start[v] ~ start[v+1]-1 구간이 v의 이웃
    int *deg = calloc(n + 2, sizeof(int));
    int *start = calloc(n + 2, sizeof(int));
    int *adj = malloc(sizeof(int) * 2 * m);
    for (int i = 0; i < m; i++) { deg[lighthouse[i][0]]++; deg[lighthouse[i][1]]++; } // 차수 계산
    for (int v = 1; v <= n; v++) start[v + 1] = start[v] + deg[v];                     // 시작 위치 누적
    int *fill = calloc(n + 2, sizeof(int));
    for (int i = 0; i < m; i++) {
        int a = lighthouse[i][0], b = lighthouse[i][1];
        adj[start[a] + fill[a]++] = b; // a -> b
        adj[start[b] + fill[b]++] = a; // b -> a
    }

    // BFS로 방문 순서와 부모 기록 (재귀 대신 사용)
    int *order = malloc(sizeof(int) * n);
    int *parent = calloc(n + 1, sizeof(int));
    int head = 0, tail = 0;
    order[tail++] = 1; // 1번 등대를 루트로
    parent[1] = -1;    // 루트 표시
    while (head < tail) {
        int v = order[head++];
        for (int k = start[v]; k < start[v + 1]; k++) {
            int u = adj[k];
            if (u == parent[v]) continue; // 부모로 되돌아가지 않음
            parent[u] = v;
            order[tail++] = u;
        }
    }

    // on[v]: v를 켤 때 최소, off[v]: v를 끌 때 최소
    int *on = malloc(sizeof(int) * (n + 1));
    int *off = malloc(sizeof(int) * (n + 1));
    for (int v = 1; v <= n; v++) { on[v] = 1; off[v] = 0; } // 리프 기준 초기값

    // BFS 역순 = 자식이 부모보다 먼저 처리됨
    for (int i = n - 1; i > 0; i--) {
        int c = order[i], p = parent[c];
        on[p] += on[c] < off[c] ? on[c] : off[c]; // 부모가 켜지면 자식은 자유
        off[p] += on[c];                          // 부모가 꺼지면 자식은 반드시 켜짐
    }
    answer = on[1] < off[1] ? on[1] : off[1]; // 루트의 두 경우 중 최솟값

    free(deg); free(start); free(adj); free(fill);
    free(order); free(parent); free(on); free(off);
    return answer;
}