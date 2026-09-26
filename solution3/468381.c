#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static const int dr[4] = {0, 1, 0, -1}; // 방향: 0=오른쪽, 1=아래, 2=왼쪽, 3=위
static const int dc[4] = {1, 0, -1, 0};
// 선로별 연결된 변(비트: 1=오른쪽, 2=아래, 4=왼쪽, 8=위)
static const int TILE[8] = {0, 5, 10, 15, 12, 9, 3, 6}; // 1─ 2│ 3# 4┘ 5└ 6┌ 7┐

static int **G, N, M;
static int *passCnt;   // 칸을 지나간 횟수
static int *passAxis;  // 첫 통과 방식: 0=가로 직진, 1=세로 직진, 2=회전
static int need, got, answer; // 고정 선로를 지나야 하는 총 횟수, 현재 지난 횟수

static void dfs(int r, int c, int d) {
    if (r < 0 || r >= N || c < 0 || c >= M) return; // 격자 밖
    int v = G[r][c], id = r * M + c;
    if (v == -1) return; // 장애물
    int in = (d + 2) % 4, axis = d % 2; // 들어온 변, 이동 축

    if (r == N - 1 && c == M - 1) { // 도착 칸
        if ((TILE[v] & (1 << in)) && got + 1 == need) answer++; // 올바른 방향 진입 + 모든 선로 통과
        return;
    }

    if (passCnt[id] > 0) { // 두 번째 통과: 직각 교차(#)만 가능
        if (passCnt[id] >= 2 || passAxis[id] != 1 - axis) return;
        if (v != 0 && v != 3) return; // 빈칸(# 설치) 또는 3번 선로만
        passCnt[id]++;
        if (v == 3) got++;
        dfs(r + dr[d], c + dc[d], d); // 직진
        if (v == 3) got--;
        passCnt[id]--;
        return;
    }

    int outs[3], k = 0;
    if (v > 0) { // 고정 선로
        int t = TILE[v];
        if (!(t & (1 << in))) return; // 들어온 방향과 연결 안 됨
        if (v == 3) outs[k++] = d; // #은 직진
        else {
            int rest = t & ~(1 << in);
            for (int o = 0; o < 4; o++) if (rest == (1 << o)) outs[k++] = o; // 나가는 방향
        }
        got++;
    } else { // 빈칸: 직진/좌회전/우회전
        outs[k++] = d; outs[k++] = (d + 1) % 4; outs[k++] = (d + 3) % 4;
    }

    passCnt[id] = 1;
    for (int i = 0; i < k; i++) {
        passAxis[id] = (outs[i] == d) ? axis : 2; // 직진이면 축 기록, 회전이면 2
        dfs(r + dr[outs[i]], c + dc[outs[i]], outs[i]);
    }
    passCnt[id] = 0;
    if (v > 0) got--;
}

// grid_rows는 2차원 배열 grid의 행 길이, grid_cols는 2차원 배열 grid의 열 길이입니다.
int solution(int** grid, size_t grid_rows, size_t grid_cols) {
    G = grid; N = (int)grid_rows; M = (int)grid_cols;
    passCnt = calloc(N * M, sizeof(int));
    passAxis = calloc(N * M, sizeof(int));
    need = got = answer = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (grid[i][j] > 0) need += (grid[i][j] == 3) ? 2 : 1; // #은 가로·세로 2번 지나야 함

    passCnt[0] = 1; passAxis[0] = 0; got = 1; // (1,1)의 1번 선로에서 오른쪽으로 출발
    dfs(0, 1, 0);

    free(passCnt); free(passAxis);
    return answer;
}