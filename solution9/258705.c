#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MOD 10007 // 문제에서 요구하는 나머지

int solution(int n, int tops[], size_t tops_len) {
    int a = 1; // 직전 칸의 마지막 삼각형이 오른쪽 마름모로 덮이지 않은 경우의 수
    int b = 0; // 직전 칸의 마지막 삼각형이 오른쪽 마름모로 덮인 경우의 수

    for (int i = 0; i < n; i++) { // 위쪽 칸마다 진행
        int na, nb; // 다음 상태
        if (tops[i]) { // 위에 삼각형이 붙은 칸
            na = (a * 3 + b * 2) % MOD; // 정삼각형/왼쪽 마름모/위 마름모 3가지 (b면 왼쪽 마름모 불가해서 2가지)
            nb = (a + b) % MOD; // 오른쪽 마름모로 끝나는 경우
        } else { // 위에 삼각형이 없는 칸
            na = (a * 2 + b) % MOD; // 정삼각형/왼쪽 마름모 2가지 (b면 1가지)
            nb = (a + b) % MOD; // 오른쪽 마름모로 끝나는 경우
        }
        a = na; // 상태 갱신
        b = nb;
    }

    return (a + b) % MOD; // 마지막 칸의 두 경우를 합산
}