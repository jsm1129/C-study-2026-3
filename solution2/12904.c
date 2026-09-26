#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// 파라미터로 주어지는 문자열은 const로 주어집니다. 변경하려면 문자열을 복사해서 사용하세요.
int solution(const char* s) {
    int answer = 1;             // 팰린드롬은 최소 길이 1(글자 하나)부터 시작
    int len = (int)strlen(s);

    // ---- 각 중심을 기준으로 좌우로 확장하며 팰린드롬 길이 확인 ----
    for (int center = 0; center < len; center++) {
        // 1) 홀수 길이 팰린드롬 (중심이 글자 하나인 경우) : "aba"
        int left = center, right = center;
        while (left >= 0 && right < len && s[left] == s[right]) {
            int cur_len = right - left + 1;
            if (cur_len > answer) answer = cur_len;
            left--;
            right++;
        }

        // 2) 짝수 길이 팰린드롬 (중심이 글자 두 개 사이인 경우) : "abba"
        left = center;
        right = center + 1;
        while (left >= 0 && right < len && s[left] == s[right]) {
            int cur_len = right - left + 1;
            if (cur_len > answer) answer = cur_len;
            left--;
            right++;
        }
    }

    return answer;
}