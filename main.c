#include "defines.h"
#include "operators.h"

static int registers[AMOUNT_REGS];

// registers (R0 ~ R9; as global variables)
// initialize their values into 0x0
static int *R0 = registers + R0_POINT;
static int *R1 = registers + R1_POINT;
static int *R2 = registers + R2_POINT;
static int *R3 = registers + R3_POINT;
static int *R4 = registers + R4_POINT;
static int *R5 = registers + R5_POINT;
static int *R6 = registers + R6_POINT;
static int *R7 = registers + R7_POINT;
static int *R8 = registers + R8_POINT;
static int *R9 = registers + R9_POINT;

void initRegisters() {
    for (int i = 0; i < AMOUNT_REGS; i++) {
        registers[i] = REG_INIT_VAL;
    }
}

typedef struct _OPERATOR {
    char opcode[OP_LENGTH];
    char op1[OP_LENGTH];
    char op2[OP_LENGTH];
}OPERATOR;

char* readFromInputFile(FILE *fp, char *buffer);

int getOperator(OPERATOR *result, char *raw);

int main(int argc, char *argv[]) {
    // initialize
    initRegisters();

    OPERATOR *expression = (OPERATOR *)malloc(sizeof(OPERATOR));

    FILE *fp = fopen("myinput.txt", "r");
    if (fp == NULL) {
        perror("File Not Found");
        return -1;
    }

    char *fullString = (char *)malloc(sizeof(char) * BUF_LENGTH); // for saving one line of input.txt

    while (feof(fp) != TRUE) {
        fullString = readFromInputFile(fp, fullString);

        if (!getOperator(expression, fullString)) {
            free(fullString);
            free(expression);
            fclose(fp);
            return -1;
        }

        // debug
        printf("%s %s %s\n", expression->opcode, expression->op1, expression->op2);
        
        memset(expression, '\0', sizeof(OPERATOR));
        memset(fullString, '\0', BUF_LENGTH);
    }

    // after all tasks end, terminate program
    free(fullString);
    fclose(fp);

    return 0;
}

char* readFromInputFile(FILE *fp, char *buffer) {
    memset(buffer, '\0', BUF_LENGTH);
    char *letter = (char *)malloc(sizeof(char) * BUF_LETTER_LENGTH); // for saving one letter of fullString
    memset(letter, '\0', BUF_LETTER_LENGTH);

    while (fread(letter, sizeof(char), BUF_LETTER_LENGTH - 1, fp) &&
           feof(fp) != TRUE &&
           strcmp(letter, "\n") != 0) {

        if (buffer[0] == '\0') {
            strcpy(buffer, letter);
        } else {
            strcat(buffer, letter);
        }
    }    

    return buffer;
}

int getOperator(OPERATOR *result, char *raw) {
    char *tok = malloc(sizeof(char) * OP_LENGTH);

    tok = strtok(raw, " ");
    if ((// essential
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

        strcpy(result->opcode, tok);
    } else {
        printf("Error: Invalid Input\n");
        return FALSE;
    }

    tok = strtok(NULL, " ");
    if (strlen(tok) == 2) {
        if (tok[0] == 'R') {
            if (tok[1] >= '0' && tok[1] <= '9') {
                strcpy(result->op1, tok);
            }
        }
    } else {
        if (strncmp(tok, "0x", 2) == 0) {
            strcpy(result->op1, tok);
        } else {
            printf("Error: Invalid Input\n");
            return FALSE;
        }
    }

    tok = strtok(NULL, " ");
    if (strlen(tok) == 2) {
        if (tok[0] == 'R') {
            if (tok[1] >= '0' && tok[1] <= '9') {
                strcpy(result->op2, tok);
            }
        }
    } else {
        if (strncmp(tok, "0x", 2) == 0) {
            strcpy(result->op2, tok);
        } else {
            printf("Error: Invalid Input\n");
            return FALSE;
        }
    }

    return TRUE;
}