#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARR_SIZE 5
#define STR_SIZE 30


void getToks() {
    int i;

    char *tok = (char *)malloc(sizeof(char) * STR_SIZE);
    char *foo = (char *)malloc(sizeof(char) * STR_SIZE);

    char *arr[10] = { NULL, };

    for (i = 0; i < ARR_SIZE; i++) {
        arr[i] = (char *)malloc(sizeof(char) * STR_SIZE);
    }

    strcpy(foo, "this is part of today's session");

    tok = strtok(foo, " ");

    i = 0;

    while (tok != NULL) {
        arr[i++] = tok;
        printf("%s ", arr[i - 1]);
        tok = strtok(NULL, " ");
    }

    for (i = 0; i < 6; i++) {
        printf("%s ", arr[i]);
    }
    printf("\n");

    free(foo);
    free(tok);
}

int main(int argc, char *argv[]) {
    char **myArr = (char **)malloc(sizeof(char *) * ARR_SIZE);

    for (int i = 0; i < ARR_SIZE; i++) {
        myArr[i] = (char *)malloc(sizeof(char) * STR_SIZE);
    }

    strcpy(myArr[0], "My String STR");
    strcpy(myArr[1], "YUM HI HELLO");
    strcpy(myArr[2], "Computer Architecture");
    strcpy(myArr[3], "Mobile Processor");
    strcpy(myArr[4], "Seehwan Yoo Proc.");

    for (int i = 0; i < ARR_SIZE; i++) {
        printf("%s\n", myArr[i]);
    }

    for (int i = 0; i < ARR_SIZE; i++) {
        free(myArr[i]);
    }

    free(myArr);

    getToks();

    return 0;
}