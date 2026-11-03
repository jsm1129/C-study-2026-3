#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int n; // 칸 수 (최대 6)
    int r[6], c[6]; // 각 칸의 좌표
} Shape;

Shape blanks[2500], pieces[2500]; // 빈칸 도형, 퍼즐 조각 도형
bool visited[50][50];
int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};

// 좌상단을 (0,0)으로 옮기고 좌표를 (r, c) 순으로 정렬
void normalize(Shape* s) {
    int minR = 100, minC = 100;
    for (int i = 0; i < s->n; i++) {
        if (s->r[i] < minR) minR = s->r[i];
        if (s->c[i] < minC) minC = s->c[i];
    }
    for (int i = 0; i < s->n; i++) { s->r[i] -= minR; s->c[i] -= minC; } // 원점 이동
    for (int i = 1; i < s->n; i++) { // 삽입 정렬
        int r = s->r[i], c = s->c[i], j = i - 1;
        while (j >= 0 && (s->r[j] > r || (s->r[j] == r && s->c[j] > c))) {
            s->r[j + 1] = s->r[j]; s->c[j + 1] = s->c[j]; j--;
        }
        s->r[j + 1] = r; s->c[j + 1] = c;
    }
}

// 90도 회전: (r, c) -> (c, -r) 후 정규화
Shape rotate(Shape s) {
    Shape t;
    t.n = s.n;
    for (int i = 0; i < s.n; i++) { t.r[i] = s.c[i]; t.c[i] = -s.r[i]; }
    normalize(&t);
    return t;
}

// 정규화된 두 도형이 같은지 비교
bool same(Shape* a, Shape* b) {
    if (a->n != b->n) return false;
    for (int i = 0; i < a->n; i++)
        if (a->r[i] != b->r[i] || a->c[i] != b->c[i]) return false;
    return true;
}

// board에서 값이 target인 연결 영역을 모두 추출해 out에 저장하고 개수 반환
int extract(int** board, int n, int target, Shape* out) {
    int cnt = 0;
    int qr[2500], qc[2500];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) visited[i][j] = false; // 방문 배열 초기화

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (visited[i][j] || board[i][j] != target) continue;
            Shape s; s.n = 0;
            int head = 0, tail = 0;
            qr[tail] = i; qc[tail++] = j; visited[i][j] = true; // BFS 시작
            while (head < tail) {
                int r = qr[head], c = qc[head++];
                s.r[s.n] = r; s.c[s.n++] = c; // 도형에 칸 추가
                for (int d = 0; d < 4; d++) {
                    int nr = r + dr[d], nc = c + dc[d];
                    if (nr < 0 || nc < 0 || nr >= n || nc >= n) continue; // 범위 밖
                    if (visited[nr][nc] || board[nr][nc] != target) continue;
                    visited[nr][nc] = true;
                    qr[tail] = nr; qc[tail++] = nc;
                }
            }
            normalize(&s);
            out[cnt++] = s;
        }
    }
    return cnt;
}

int solution(int** game_board, size_t game_board_rows, size_t game_board_cols, int** table, size_t table_rows, size_t table_cols) {
    int n = (int)game_board_rows;
    int bc = extract(game_board, n, 0, blanks); // 빈칸 영역 추출
    int pc = extract(table, n, 1, pieces); // 퍼즐 조각 추출

    bool used[2500] = {false}; // 사용한 조각 표시
    int answer = 0;

    for (int i = 0; i < bc; i++) {
        for (int j = 0; j < pc; j++) {
            if (used[j] || pieces[j].n != blanks[i].n) continue; // 사용했거나 크기가 다르면 건너뜀
            Shape p = pieces[j];
            bool ok = false;
            for (int k = 0; k < 4; k++) { // 4방향 회전 시도
                if (same(&blanks[i], &p)) { ok = true; break; }
                p = rotate(p);
            }
            if (ok) {
                used[j] = true;
                answer += blanks[i].n; // 채운 칸 수 누적
                break; // 이 빈칸은 채웠으니 다음 빈칸으로
            }
        }
    }
    return answer;
}