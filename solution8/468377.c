#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int solution(int** cost, size_t cost_rows, size_t cost_cols, int** hint, size_t hint_rows, size_t hint_cols) {
    int n = (int)cost_rows; // 스테이지 수
    int answer = -1; // 최소 총비용 (-1 = 아직 없음)
    int cnt[16]; // cnt[i] = i번 스테이지(0-index)에 쓸 수 있는 힌트권 수

    for (int mask = 0; mask < (1 << hint_rows); mask++) { // 각 번들 구매 여부 완전탐색
        int total = 0; // 이번 경우의 총비용
        for (int i = 0; i < n; i++) cnt[i] = 0; // 힌트권 개수 초기화

        for (int i = 0; i < (int)hint_rows; i++) {
            if (!(mask >> i & 1)) continue; // i+1번 스테이지 번들 안 삼
            total += hint[i][0]; // 번들 가격 지불
            for (int j = 1; j < (int)hint_cols; j++) cnt[hint[i][j] - 1]++; // 힌트권 번호(1-index)별로 개수 추가
        }

        for (int i = 0; i < n; i++) {
            int use = cnt[i] < n - 1 ? cnt[i] : n - 1; // 최대 n-1장까지만 사용 가능
            total += cost[i][use]; // 많이 쓸수록 싸므로 가진 만큼 다 사용
        }

        if (answer == -1 || total < answer) answer = total; // 최솟값 갱신
    }
    return answer;
}