#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static int** S; // qsort 비교용 전역 포인터

// 근무 태도 내림차순, 같으면 동료 평가 오름차순
static int cmp(const void* x, const void* y) {
    int i = *(const int*)x, j = *(const int*)y;
    if (S[i][0] != S[j][0]) return S[j][0] - S[i][0];
    return S[i][1] - S[j][1];
}

// scores_rows는 2차원 배열 scores의 행 길이, scores_cols는 2차원 배열 scores의 열 길이입니다.
int solution(int** scores, size_t scores_rows, size_t scores_cols) {
    int n = (int)scores_rows;
    int* idx = malloc(sizeof(int) * n);
    bool* out = calloc(n, sizeof(bool)); // 인센티브 제외 여부
    for (int i = 0; i < n; i++) idx[i] = i;
    S = scores;
    qsort(idx, n, sizeof(int), cmp);

    int maxB = 0; // 지금까지 본(근무 태도가 더 높거나 같은) 사원들의 최대 동료 평가
    for (int k = 0; k < n; k++) {
        int i = idx[k];
        if (scores[i][1] < maxB) out[i] = true; // 두 점수 모두 낮은 사원이 존재 → 제외
        else maxB = scores[i][1];
    }

    int answer;
    if (out[0]) answer = -1; // 완호가 제외되면 -1
    else {
        int w = scores[0][0] + scores[0][1]; // 완호의 합산 점수
        answer = 1;
        for (int i = 1; i < n; i++)
            if (!out[i] && scores[i][0] + scores[i][1] > w) answer++; // 완호보다 합이 큰 사원 수 + 1
    }

    free(idx); free(out);
    return answer;
}