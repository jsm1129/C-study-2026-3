#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// qsort용 오름차순 비교 함수
static int cmp(const void* a, const void* b) {
    long long x = *(const long long*)a, y = *(const long long*)b; // void* -> long long 값으로 변환
    return (x > y) - (x < y); // x<y면 -1, 같으면 0, x>y면 1 (빼기 대신 써서 오버플로 방지)
}

char* solution(long long n, const char* bans[], size_t bans_len) {
    long long* idx = (long long*)malloc(sizeof(long long) * bans_len); // 금지 주문의 번호 저장 배열

    for (size_t i = 0; i < bans_len; i++) { // 금지 주문마다
        long long v = 0; // 이 주문의 번호
        for (const char* p = bans[i]; *p; p++) // 문자 하나씩 순회
            v = v * 26 + (*p - 'a' + 1); // 1부터 시작하는 26진법 (a=1, z=26, aa=27)
        idx[i] = v; // 번호 저장
    }

    qsort(idx, bans_len, sizeof(long long), cmp); // 번호 오름차순 정렬

    for (size_t i = 0; i < bans_len; i++) // 작은 번호부터 확인
        if (idx[i] <= n) n++; // 현재 n 이하에 금지 주문이 있으면 답이 한 칸 뒤로 밀림
    free(idx); // 번호 배열 해제

    char buf[16]; // 결과 문자열 임시 버퍼 (최대 11글자 + 여유)
    int len = 0; // 결과 길이
    while (n > 0) { // 번호 -> 문자열 변환
        n--; // 1-based를 0-based로 보정 (z 처리용)
        buf[len++] = 'a' + n % 26; // 끝자리 문자부터 저장
        n /= 26; // 다음 자리로 이동
    }

    char* answer = (char*)malloc(len + 1); // 결과 문자열 + '\0'
    for (int i = 0; i < len; i++)
        answer[i] = buf[len - 1 - i]; // 거꾸로 저장했으니 뒤집어서 복사
    answer[len] = '\0'; // 문자열 끝 표시
    return answer; // 결과 반환
}