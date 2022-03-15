#include "operators.h"

// read "input" string from ./input.txt
// 함수를 한 번 호출하면, input.txt 한 줄에서 공백으로 구분된 문자열들로 이루어진 배열을 반환한다.
char** getOneOperationFromTxt(FILE *fp, char *buf[]) {
    char *rawBuf = (char *)malloc(sizeof(char) * BUF_LENGTH);
    char *recv = (char *)malloc(sizeof(char) * 2);

    memset(rawBuf, '\0', BUF_LENGTH);

    while (fread(recv, sizeof(char), 1, fp) &&
            feof(fp) != TRUE &&
            strcmp(recv, "\n") != 0) {

        if (rawBuf[0] == '\0') {
            strcpy(rawBuf, recv);
        } else {
            strcat(rawBuf, recv);
        }

        memset(recv, '\0', 2);
    }

    free(recv); // free recv variable

    // debug: print result
    printf("%s\n", rawBuf);


    // TO DO: 위에서 가져온 명령어 한 줄을 공백을 기준으로 여러 개의 토큰으로 쪼갠 후,
    // 이들을 문자열 배열 buf에 순차대로 저장한다. 저장이 끝나면 buf를 반환한다.
    int idx = 0;
    char *tok = (char *)malloc(sizeof(char) * BUF_LENGTH);

    printf("%s\n", rawBuf); // debug

    tok = strtok(rawBuf, " ");

    printf("%s ", tok); // debug

    strcpy(buf[idx++], tok);

    printf("%s ", tok); // debug
    while (tok != NULL && idx < BUF_LENGTH) {
        tok = strtok(NULL, " ");
        printf("%s ", tok); // debug
        strcpy(buf[idx++], tok);
    }

    // debug: print all tokens
    for (int i = 0; i < idx; i++) {
        printf("%s | ", buf[i]);
    }
    printf("\n");

    free(tok);

    free(rawBuf);

    return buf; // return array of tokens
}

// assign value "val" into target "op1"
void mov(int *op, int val) {
    *op = val;
}; 

int add(int op1, int op2); // 

int subtract(int op1, int op2);

int multiply(int op1, int op2);

int divide(int op1, int op2);