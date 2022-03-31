#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

#define OP_LENGTH 10

typedef struct _OPERATOR {
    char opcode[OP_LENGTH];
    char op1[OP_LENGTH];
    char op2[OP_LENGTH];
}OPERATOR;

int getOperator(OPERATOR *foo, char *raw);

int main() {
    OPERATOR *foo = (OPERATOR *)malloc(sizeof(OPERATOR));
    char *raw = (char *)malloc(sizeof(char) * OP_LENGTH);

    strcpy(raw, "M R 0x3");

    if (!getOperator(foo, raw)) {
        free(raw);
        free(foo);
        return 1;
    };

    printf("%s %s %s\n", foo->opcode, foo->op1, foo->op2);

    free(foo);

    return 0;
}

int getOperator(OPERATOR *foo, char *raw) {
    char *tok = malloc(sizeof(char) * OP_LENGTH);

    tok = strtok(raw, " ");
    if ((
        // essential
         strcmp(tok, "M") == 0 ||  // mov
         strcmp(tok, "+") == 0 ||  // add
         strcmp(tok, "-") == 0 ||  // sub
         strcmp(tok, "*") == 0 ||  // mul
         strcmp(tok, "/") == 0 ||  // div

        // additional
         strcmp(tok, "H") == 0 ||  // halt
         strcmp(tok, "J") == 0 ||  // jump
         strcmp(tok, "C") == 0 ||  // compare
         strcmp(tok, "B") == 0)) { // branch

        strcpy(foo->opcode, tok);
    } else {
        printf("Error: Invalid Input\n");
        return FALSE;
    }

    tok = strtok(NULL, " ");
    if (strlen(tok) == 2) {
        if (tok[0] == 'R') {
            if (tok[1] >= '0' && tok[1] <= '9') {
                strcpy(foo->op1, tok);
            }
        }
    } else {
        if (strncmp(tok, "0x", 2) == 0) {
            strcpy(foo->op1, tok);
        } else {
            printf("Error: Invalid Input\n");
            return FALSE;
        }
    }

    tok = strtok(NULL, " ");
    if (strlen(tok) == 2) {
        if (tok[0] == 'R') {
            if (tok[1] >= '0' && tok[1] <= '9') {
                strcpy(foo->op2, tok);
            }
        }
    } else {
        if (strncmp(tok, "0x", 2) == 0) {
            strcpy(foo->op2, tok);
        } else {
            printf("Error: Invalid Input\n");
            return FALSE;
        }
    }

    return TRUE;
}