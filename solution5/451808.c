#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

extern char* submit(int);

static int qd[9000][4];             // 질문 1000~9999의 자릿수
static int sd[3024][4], sm[3024];   // 후보 비밀번호 자릿수, 포함 숫자 비트마스크
static int sv[3024];                // 후보 비밀번호 값
static int cand[3024], cn;          // 아직 가능한 후보 인덱스

static int score(int q, int s) {    // 질문 q를 후보 s에 냈을 때 단서 (S*5+B)
    int st = 0, b = 0;
    for (int k = 0; k < 4; k++) {
        if (qd[q][k] == sd[s][k]) st++;              // 자리까지 맞음 → S
        else if (sm[s] >> qd[q][k] & 1) b++;         // 숫자만 포함 → B
    }
    return st * 5 + b;
}

int solution(int n) {
    int sc = 0;
    for (int x = 1000; x <= 9999; x++) {
        int d[4] = {x / 1000, x / 100 % 10, x / 10 % 10, x % 10}, m = 0, ok = 1;
        for (int k = 0; k < 4; k++) qd[x - 1000][k] = d[k];                 // 질문 자릿수 저장
        for (int k = 0; k < 4; k++) { if (!d[k] || (m >> d[k] & 1)) ok = 0; m |= 1 << d[k]; } // 0 또는 중복 검사
        if (ok) { for (int k = 0; k < 4; k++) sd[sc][k] = d[k]; sm[sc] = m; sv[sc++] = x; } // 1~9 서로 다른 수만 후보
    }
    cn = sc;
    for (int i = 0; i < cn; i++) cand[i] = i;                                // 처음엔 전부 후보

    while (cn > 1) {
        int best = 0, bw = 1 << 30, bin = 0;
        for (int q = 0; q < 9000; q++) {                                     // 모든 질문 시도
            int cnt[25] = {0}, w = 0;
            for (int i = 0; i < cn && w <= bw; i++) {                        // 단서별 후보 수 세기
                int r = score(q, cand[i]);
                if (++cnt[r] > w) w = cnt[r];                                // 최악의 그룹 크기
            }
            if (w > bw) continue;                                            // 지금 최선보다 나쁘면 버림
            int in = 0;
            for (int i = 0; i < cn; i++) if (sv[cand[i]] == q + 1000) { in = 1; break; } // 후보 자체인 질문인지
            if (w < bw || (in && !bin)) { bw = w; best = q; bin = in; }      // 동점이면 정답 가능성 있는 쪽 우대
        }
        char* r = submit(best + 1000);                                       // 질문 제출
        int res = (r[0] - '0') * 5 + (r[3] - '0');                          // "xS yB" 파싱
        if (res == 20) return best + 1000;                                   // 4S면 바로 정답
        int m = 0;
        for (int i = 0; i < cn; i++)
            if (score(best, cand[i]) == res) cand[m++] = cand[i];            // 단서와 맞는 후보만 남김
        cn = m;
    }
    return sv[cand[0]];                                                      // 하나 남은 후보가 정답 (제출 불필요)
}