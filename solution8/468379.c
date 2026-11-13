#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

int* solution(int m, int n, int h, int w, int** drops, size_t drops_rows, size_t drops_cols) {
    int W = n - w + 1, H = m - h + 1; // 구역 왼쪽 위가 올 수 있는 열/행 개수

    int* t = (int*)malloc(sizeof(int) * m * n); // t[r*n+c] = 그 칸에 비가 떨어지는 순서
    for (int i = 0; i < m * n; i++) t[i] = INT_MAX; // 비 안 맞는 칸 = 무한대
    for (size_t i = 0; i < drops_rows; i++) t[drops[i][0] * n + drops[i][1]] = (int)i + 1; // i+1번째 빗방울

    int* rowMin = (int*)malloc(sizeof(int) * m * W); // rowMin[r][c] = r행의 c~c+w-1 구간 최솟값
    int* dq = (int*)malloc(sizeof(int) * (m > n ? m : n)); // 인덱스 저장용 덱 (배열로 구현)

    // 1단계: 각 행마다 가로 w 슬라이딩 윈도우 최솟값
    for (int r = 0; r < m; r++) {
        int* a = t + r * n; // r행 시작 주소
        int hd = 0, tl = 0; // 덱 head, tail
        for (int c = 0; c < n; c++) {
            while (tl > hd && a[dq[tl - 1]] >= a[c]) tl--; // 나보다 큰 값은 최솟값 될 일 없으니 제거
            dq[tl++] = c; // 현재 열 추가
            if (dq[hd] <= c - w) hd++; // 윈도우 밖으로 나간 앞쪽 제거
            if (c >= w - 1) rowMin[r * W + c - w + 1] = a[dq[hd]]; // 덱 맨 앞 = 윈도우 최솟값
        }
    }

    int* best = (int*)malloc(sizeof(int) * H * W); // best[r][c] = (r,c)가 왼쪽 위인 h×w 구역의 최솟값

    // 2단계: rowMin의 각 열마다 세로 h 슬라이딩 윈도우 최솟값
    for (int c = 0; c < W; c++) {
        int hd = 0, tl = 0; // 덱 초기화
        for (int r = 0; r < m; r++) {
            while (tl > hd && rowMin[dq[tl - 1] * W + c] >= rowMin[r * W + c]) tl--; // 큰 값 제거
            dq[tl++] = r; // 현재 행 추가
            if (dq[hd] <= r - h) hd++; // 윈도우 밖 행 제거
            if (r >= h - 1) best[(r - h + 1) * W + c] = rowMin[dq[hd] * W + c]; // 구역 최솟값 = 처음 비 맞는 시점
        }
    }

    int* answer = (int*)malloc(sizeof(int) * 2); // 반환할 [행, 열]
    answer[0] = answer[1] = 0; // 기본값 (0,0)
    int bt = -1; // 지금까지 가장 늦게 비 맞는 시점

    // 3단계: 가장 늦게 비 맞는 구역 선택
    for (int r = 0; r < H; r++)
        for (int c = 0; c < W; c++)
            if (best[r * W + c] > bt) { bt = best[r * W + c]; answer[0] = r; answer[1] = c; } // 행→열 순 순회 + '>' 비교라 동점 시 위/왼쪽 우선

    free(t); free(rowMin); free(dq); free(best); // 임시 메모리 해제
    return answer;
}