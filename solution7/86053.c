#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

long long solution(int a, int b, int g[], size_t g_len, int s[], size_t s_len, int w[], size_t w_len, int t[], size_t t_len) {
    long long lo = 0, hi = 1000000000000000LL; // 1e15: 최악 4e14보다 크고, cap 계산 시 오버플로 없음
    long long answer = hi;

    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2; // 검사할 시간
        long long gold = 0, silver = 0, total = 0;

        for (size_t i = 0; i < g_len; i++) {
            long long cnt = (mid + t[i]) / (2LL * t[i]); // 편도 운반 횟수
            long long cap = cnt * w[i]; // 최대 5e14 * 100 = 5e16 → 안전

            gold += (g[i] < cap) ? g[i] : cap; // 금만 옮길 때 최대량
            silver += (s[i] < cap) ? s[i] : cap; // 은만 옮길 때 최대량
            total += ((long long)g[i] + s[i] < cap) ? (long long)g[i] + s[i] : cap; // 금+은 합친 최대량
        }

        if (gold >= a && silver >= b && total >= (long long)a + b) { // 세 조건 모두 만족
            answer = mid;
            hi = mid - 1; // 더 짧은 시간 탐색
        } else {
            lo = mid + 1; // 시간 늘림
        }
    }
    return answer;
}