#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);  // 오름차순 정렬용 비교 함수
}

// A_len은 배열 A의 길이입니다.
// B_len은 배열 B의 길이입니다.
int solution(int A[], size_t A_len, int B[], size_t B_len) {
    int answer = 0;

    qsort(A, A_len, sizeof(int), cmp);  // A팀 오름차순 정렬
    qsort(B, B_len, sizeof(int), cmp);  // B팀 오름차순 정렬

    size_t i = 0;  // A팀에서 아직 못 이긴 값 중 최솟값의 인덱스

    for (size_t j = 0; j < B_len; j++) {   // B팀을 작은 값부터 순회
        if (i < A_len && B[j] > A[i]) {    // 현재 B값이 A의 최솟값을 이기면
            answer++;                      // 승점 획득
            i++;                           // 이긴 A는 처리 완료, 다음 A로
        }
        // 못 이기면 이 B값은 버리고 다음 B값 시도
    }

    return answer;
}