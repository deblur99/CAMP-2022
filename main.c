#include "defines.h"

static int registers[AMOUNT_REGS];

// to be deleted below varables (R0 ~ R9)
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

int executeExpression(OPERATOR *expression);

void showCalcResult(OPERATOR *expression, int result);

int mov(OPERATOR *expression);

int add(OPERATOR *expression);

int subtract(OPERATOR *expression);

int multiply(OPERATOR *expression);

int divide(OPERATOR *expression);

int halt(OPERATOR *expression);

int jump(OPERATOR *expression);

int compare(OPERATOR *expression);

int branch(OPERATOR *expression);

int main(int argc, char *argv[]) {
    initRegisters();
    *R1 = 10; // debug
    printf("%d\n", *R1); // debug

    char *fullString = (char *)malloc(sizeof(char) * BUF_LENGTH); // for saving one line of input.txt
    OPERATOR *expression = (OPERATOR *)malloc(sizeof(OPERATOR));
    FILE *fp = fopen("myinput.txt", "r");
    if (fp == NULL) {
        perror("File Not Found");
        return -1;
    }

    // running actual task
    while (feof(fp) != TRUE) {
        fullString = readFromInputFile(fp, fullString);

        if (!getOperator(expression, fullString)) {
            free(fullString);
            free(expression);
            fclose(fp);
            return -1;
        }

        printf("%s %s %s ", expression->opcode, expression->op1, expression->op2);

        // TO DO : call executing function with expression struct argument
        // and process it on the called function by value of the argument
        if (!executeExpression(expression)) {
            free(fullString);
            free(expression);
            fclose(fp);
            return -1;
        }

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

int executeExpression(OPERATOR *expression) {
    switch (*(expression->opcode)) {
        case 'M':
            showCalcResult(expression, mov(expression));
            break;
        
        case '+':
            showCalcResult(expression, add(expression));
            break;
            
        case '-':
            showCalcResult(expression, subtract(expression));
            break;
            
        case '*':
            showCalcResult(expression, multiply(expression));
            break;
            
        case '/':
            showCalcResult(expression, divide(expression));
            break;
        
        default:
            printf("Error: invalid opcode\n");
            return FALSE;
    }

    return TRUE;
}

int mov(OPERATOR *expression) {
    if (*expression->op1 != 'R') {
        return -1;
    }

    *(registers + expression->op1[1]) = strtol(expression->op2, NULL, 16);

    return (int)strtol(expression->op2, NULL, 16);
}

int add(OPERATOR *expression) {
    // 1) Rn + Rm -> Rn
    // 2) Rn + number -> Rn
    // 3) number + number -> R0

    // 1) case
    if (expression->op1[0] == 'R' && expression->op2[0] == 'R') {
        *registers = *(registers + expression->op1[1]) + *(registers + expression->op2[1]);
        return *(registers + expression->op1[1]) + *(registers + expression->op2[1]);
    }

    // 2) case
    if (expression->op1[0] == 'R' && expression->op2[0] != 'R') {
        *(registers + expression->op1[1]) += strtol(expression->op2, NULL, 16);
        return *(registers + expression->op1[1]);
    } else if (expression->op1[0] != 'R' && expression->op2[0] == 'R') {
        *(registers + expression->op2[1]) += strtol(expression->op1, NULL, 16);
        return *(registers + expression->op2[1]);
    }

    // 3) case
    if (expression->op1[0] != 'R' && expression->op2[0] != 'R') {
        *registers = strtol(expression->op1, NULL, 16) + strtol(expression->op2, NULL, 16);
        return *registers;
    }

    return 0;
}

int subtract(OPERATOR *expression) {
    return TRUE;
}

int multiply(OPERATOR *expression) {
    return TRUE;
}

int divide(OPERATOR *expression) {
    return TRUE;
}

int halt(OPERATOR *expression) {
    return TRUE;
}

int jump(OPERATOR *expression) {
    return TRUE;
}

int compare(OPERATOR *expression) {
    return TRUE;
}

int branch(OPERATOR *expression) {
    return TRUE;
}

void showCalcResult(OPERATOR *expression, int result) {
    if (expression->op1[0] != 'R' && expression->op1[0] != 'R') {
        if (strncmp(expression->opcode, "M", 1) != 0) {
            printf("=> R0: 0x%X = 0x%lX %s 0x%lX\n", 
                result, 
                *(registers + expression->op1[1]) - strtol(expression->op2, NULL, 16),
                expression->opcode, strtol(expression->op2, NULL, 16));
        } else {
            printf("=> R0: 0x%X\n", result);
        }
    } else {
        if (strncmp(expression->opcode, "M", 1) != 0) {
            printf("=> %s: 0x%X = 0x%lX %s 0x%lX\n",
                expression->op1, result, 
                *(registers + expression->op1[1]) - strtol(expression->op2, NULL, 16),
                expression->opcode, strtol(expression->op2, NULL, 16));
        } else {
            printf("=> %s: 0x%X\n", expression->op1, result);
        }
    }
}