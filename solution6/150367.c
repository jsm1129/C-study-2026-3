#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int bits[64]; // 포화 이진트리를 중위순회 순서로 펼친 비트 배열

// [l, r] 구간이 하나의 서브트리, zero는 조상 중 더미(0) 노드가 있는지
bool dfs(int l, int r, bool zero) {
    if (l > r) return true; // 빈 서브트리는 항상 가능
    int mid = (l + r) / 2; // 구간의 가운데가 서브트리의 루트
    if (zero && bits[mid] == 1) return false; // 더미 노드 아래에 실제 노드(1)가 있으면 불가능
    bool next = zero || bits[mid] == 0; // 현재 노드가 더미면 자식들도 모두 더미여야 함
    return dfs(l, mid - 1, next) && dfs(mid + 1, r, next); // 왼쪽, 오른쪽 서브트리 검사
}

int* solution(long long numbers[], size_t numbers_len) {
    int* answer = (int*)malloc(sizeof(int) * numbers_len); // 결과 배열 할당
    for (size_t t = 0; t < numbers_len; t++) {
        long long n = numbers[t];

        int len = 0;
        while ((n >> len) > 0) len++; // 이진수의 비트 길이 계산

        int size = 1;
        while (size < len) size = size * 2 + 1; // len 이상인 가장 작은 2^k-1 (1, 3, 7, 15, ...)

        for (int i = 0; i < size; i++) bits[i] = 0; // 배열 초기화 (앞쪽 패딩용 0)
        for (int i = 0; i < len; i++) bits[size - 1 - i] = (n >> i) & 1; // 하위 비트부터 뒤에서 채움

        answer[t] = dfs(0, size - 1, false) ? 1 : 0; // 전체 트리 검사, 루트 위엔 더미 없음
    }
    return answer;
}