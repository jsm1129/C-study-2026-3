#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// sequence_len은 배열 sequence의 길이입니다.
long long solution(int sequence[], size_t sequence_len) {
    // 펄스는 [1,-1,1,-1...] 또는 [-1,1,-1,1...] 두 종류뿐이고,
    // 시작 위치와 무관하게 "어느 펄스를 쓸지"는 자유롭게 고를 수 있다.
    // sequence[i]에 (-1)^i 를 미리 곱해둔 배열 b를 만들면,
    // 원하는 구간의 펄스곱 합은 결국 b의 부분합(±)으로 환원된다.
    // 즉 답 = max( b의 최대 부분수열 합, b의 최소 부분수열 합의 절댓값 )
    long long *b = (long long*)malloc(sequence_len * sizeof(long long));
    for (size_t i = 0; i < sequence_len; i++) {
        // 짝수 인덱스는 그대로, 홀수 인덱스는 부호 반전
        b[i] = (i % 2 == 0) ? (long long)sequence[i] : -(long long)sequence[i];
    }

    // Kadane 알고리즘으로 최대 부분수열 합 구하기
    long long max_ending_here = b[0];
    long long max_so_far = b[0];
    for (size_t i = 1; i < sequence_len; i++) {
        // 이전까지의 합에 현재 원소를 더할지, 현재 원소부터 새로 시작할지 선택
        max_ending_here = (max_ending_here + b[i] > b[i]) ? max_ending_here + b[i] : b[i];
        if (max_ending_here > max_so_far) max_so_far = max_ending_here;
    }

    // 같은 방식으로 최소 부분수열 합 구하기 (반대 부호 펄스에 해당)
    long long min_ending_here = b[0];
    long long min_so_far = b[0];
    for (size_t i = 1; i < sequence_len; i++) {
        min_ending_here = (min_ending_here + b[i] < b[i]) ? min_ending_here + b[i] : b[i];
        if (min_ending_here < min_so_far) min_so_far = min_ending_here;
    }

    free(b);

    // 최소합의 절댓값과 최대합 중 더 큰 값이 정답 (두 펄스 방향 중 더 나은 쪽)
    long long candidate1 = max_so_far;
    long long candidate2 = -min_so_far;

    long long answer = (candidate1 > candidate2) ? candidate1 : candidate2;
    return answer;
}