#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define INF 0x3f3f3f3f // 도달 불가능한 상태를 나타내는 큰 값

// 파라미터로 주어지는 문자열은 const로 주어집니다. 변경하려면 문자열을 복사해서 사용하세요.
int solution(const char* numbers) {
    int answer = INF;

    // 각 숫자의 키패드 좌표 (행, 열), 인덱스 = 숫자
    int pos[10][2] = {{3,1},{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}};

    // cost[a][b]: 손가락을 a에서 b로 옮겨 누르는 가중치
    int cost[10][10];
    for (int a = 0; a < 10; a++) {
        for (int b = 0; b < 10; b++) {
            if (a == b) { cost[a][b] = 1; continue; } // 같은 키를 다시 누르면 1
            int dr = abs(pos[a][0] - pos[b][0]);     // 행 차이
            int dc = abs(pos[a][1] - pos[b][1]);     // 열 차이
            int mn = dr < dc ? dr : dc;               // 대각선으로 이동할 칸 수
            int mx = dr > dc ? dr : dc;
            cost[a][b] = mn * 3 + (mx - mn) * 2;      // 대각선 3, 나머지 직선 2
        }
    }

    // dp[l][r]: 왼손이 l, 오른손이 r에 있을 때까지의 최소 가중치
    int dp[10][10], nd[10][10];
    memset(dp, 0x3f, sizeof(dp)); // 전부 INF로 초기화
    dp[4][6] = 0;                 // 시작: 왼손 4, 오른손 6

    for (int k = 0; numbers[k]; k++) {
        int d = numbers[k] - '0';     // 이번에 눌러야 할 숫자
        memset(nd, 0x3f, sizeof(nd)); // 다음 단계 테이블 초기화

        for (int l = 0; l < 10; l++) {
            for (int r = 0; r < 10; r++) {
                if (dp[l][r] == INF) continue; // 도달 불가능한 상태는 건너뜀
                int v;

                // 왼손이 d를 누름 (오른손이 d 위에 있으면 불가)
                if (d != r) {
                    v = dp[l][r] + cost[l][d];
                    if (v < nd[d][r]) nd[d][r] = v;
                }

                // 오른손이 d를 누름 (왼손이 d 위에 있으면 불가)
                if (d != l) {
                    v = dp[l][r] + cost[r][d];
                    if (v < nd[l][d]) nd[l][d] = v;
                }
            }
        }
        memcpy(dp, nd, sizeof(dp)); // 다음 단계로 갱신
    }

    // 마지막 숫자까지 누른 모든 상태 중 최솟값
    for (int l = 0; l < 10; l++)
        for (int r = 0; r < 10; r++)
            if (dp[l][r] < answer) answer = dp[l][r];

    return answer;
}