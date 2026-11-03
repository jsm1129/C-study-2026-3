#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define N 2500 // 50 x 50 셀 개수

int parent[N]; // 각 셀이 속한 그룹의 부모
char val[N][11]; // 루트 셀에만 의미 있는 값 저장 (빈 문자열 = EMPTY)

int idx(int r, int c) { return (r - 1) * 50 + (c - 1); } // (r, c) -> 1차원 인덱스

int find(int x) {
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]); // 경로 압축
}

char** solution(const char* commands[], size_t commands_len) {
    char** answer = (char**)malloc(sizeof(char*) * commands_len); // PRINT 최대 개수만큼 할당
    int cnt = 0;

    for (int i = 0; i < N; i++) { parent[i] = i; val[i][0] = '\0'; } // 초기화: 모두 독립, 빈 값

    for (size_t t = 0; t < commands_len; t++) {
        char op[10], a[11], b[11], c[11], d[11];
        int k = sscanf(commands[t], "%s %s %s %s %s", op, a, b, c, d); // 토큰 개수로 명령 형태 구분

        if (strcmp(op, "UPDATE") == 0 && k == 4) { // UPDATE r c value
            int x = find(idx(atoi(a), atoi(b)));
            strcpy(val[x], c); // 그룹 루트의 값 변경
        }
        else if (strcmp(op, "UPDATE") == 0 && k == 3) { // UPDATE value1 value2
            for (int i = 0; i < N; i++)
                if (parent[i] == i && strcmp(val[i], a) == 0) strcpy(val[i], b); // 루트 셀만 검사
        }
        else if (strcmp(op, "MERGE") == 0) { // MERGE r1 c1 r2 c2
            int x = find(idx(atoi(a), atoi(b)));
            int y = find(idx(atoi(c), atoi(d)));
            if (x == y) continue; // 이미 같은 그룹이면 무시
            if (val[x][0] == '\0') strcpy(val[x], val[y]); // 첫 셀이 비어 있으면 두 번째 값 사용
            parent[y] = x; // y 그룹을 x 그룹에 합침
            val[y][0] = '\0'; // 루트가 아니게 된 셀의 값 제거
        }
        else if (strcmp(op, "UNMERGE") == 0) { // UNMERGE r c
            int p = idx(atoi(a), atoi(b));
            int root = find(p);
            char saved[11];
            strcpy(saved, val[root]); // 그룹 값 백업

            int members[N], m = 0;
            for (int i = 0; i < N; i++)
                if (find(i) == root) members[m++] = i; // 같은 그룹 셀을 먼저 모두 수집
            for (int i = 0; i < m; i++) {
                parent[members[i]] = members[i]; // 각자 독립 그룹으로 분리
                val[members[i]][0] = '\0'; // 값 초기화
            }
            strcpy(val[p], saved); // 지정한 셀만 원래 값 유지
        }
        else if (strcmp(op, "PRINT") == 0) { // PRINT r c
            int x = find(idx(atoi(a), atoi(b)));
            char* s = (char*)malloc(11);
            strcpy(s, val[x][0] ? val[x] : "EMPTY"); // 비어 있으면 EMPTY
            answer[cnt++] = s;
        }
    }
    return answer;
}
