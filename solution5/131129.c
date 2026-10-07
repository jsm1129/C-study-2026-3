#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int* solution(int target) {
    int* cnt = (int*)malloc(sizeof(int) * (target + 1)); // cnt[i]: 점수 i를 만드는 최소 다트 수
    int* sb = (int*)malloc(sizeof(int) * (target + 1));  // sb[i]: 그때 싱글+불 최대 횟수
    cnt[0] = 0; sb[0] = 0;                               // 0점은 다트 0개

    for (int i = 1; i <= target; i++) {                  // 1점부터 target까지 차례로 계산
        cnt[i] = 1e9; sb[i] = 0;                         // 아직 방법이 없으니 큰 값으로 초기화

        for (int s = 1; s <= 21; s++) {                  // 싱글 1~20 + 불(21번째로 처리)
            int v = (s == 21) ? 50 : s;                  // 21이면 불(50점), 아니면 싱글 점수
            if (v > i) continue;                         // 현재 점수보다 크면 못 던짐
            int c = cnt[i - v] + 1;                      // 다트 1개 추가
            int b = sb[i - v] + 1;                       // 싱글/불이므로 카운트도 +1
            if (c < cnt[i] || (c == cnt[i] && b > sb[i])) { // 다트 적은 쪽, 같으면 싱글/불 많은 쪽
                cnt[i] = c; sb[i] = b;                   // 더 좋은 방법으로 갱신
            }
        }

        for (int s = 1; s <= 20; s++) {                  // 과녁 숫자 1~20
            for (int m = 2; m <= 3; m++) {               // m=2 더블, m=3 트리플
                int v = s * m;                           // 실제 얻는 점수
                if (v > i) continue;                     // 현재 점수보다 크면 못 던짐
                int c = cnt[i - v] + 1;                  // 다트 1개 추가
                int b = sb[i - v];                       // 더블/트리플은 싱글/불 카운트 그대로
                if (c < cnt[i] || (c == cnt[i] && b > sb[i])) { // 같은 비교 기준 적용
                    cnt[i] = c; sb[i] = b;               // 더 좋은 방법으로 갱신
                }
            }
        }
    }

    int* answer = (int*)malloc(sizeof(int) * 2);         // 결과 [다트 수, 싱글/불 횟수]
    answer[0] = cnt[target];                             // 최소 다트 수
    answer[1] = sb[target];                              // 최대 싱글/불 횟수
    free(cnt); free(sb);                                 // DP 배열 메모리 해제
    return answer;
}