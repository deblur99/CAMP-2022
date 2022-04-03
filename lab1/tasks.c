#include "tasks.h"

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

int getOperator(OPERATOR *expression, char *raw) {
    char *tok = malloc(sizeof(char) * OP_LENGTH);

    tok = strtok(raw, " ");
    if ((// essential
         strcmp(tok, "M") == 0 ||  // mov
         strcmp(tok, "+") == 0 ||  // add
         strcmp(tok, "-") == 0 ||  // sub
         strcmp(tok, "*") == 0 ||  // mul
         strcmp(tok, "/") == 0 ||  // div
         strcmp(tok, "H") == 0 ||  // halt
         // additional         
         strcmp(tok, "C") == 0  // compare
        )) { // branch

        strcpy(expression->opcode, tok);
    } else {
        printf("Error: Invalid Input\n");
        return FALSE;
    }

    tok = strtok(NULL, " ");
    if (strlen(tok) == 2) {
        if (tok[0] == 'R') {
            if (tok[1] >= '0' && tok[1] <= '9') {
                strcpy(expression->op1, tok);
            }
        }
    } else {
        if (strncmp(tok, "0x", 2) == 0) {
            strcpy(expression->op1, tok);
        } else {
            printf("Error: Invalid Input\n");
            return FALSE;
        }
    }

    tok = strtok(NULL, " ");
    if (strlen(tok) == 2) {
        if (tok[0] == 'R') {
            if (tok[1] >= '0' && tok[1] <= '9') {
                strcpy(expression->op2, tok);
            }
        }
    } else {
        if (strncmp(tok, "0x", 2) == 0) {
            strcpy(expression->op2, tok);
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
            return showCalcResult(expression, mov(expression));
        
        case '+':
            return showCalcResult(expression, add(expression));
            
        case '-':
            return showCalcResult(expression, subtract(expression));
            
        case '*':
            return showCalcResult(expression, multiply(expression));
            
        case '/':
            return showCalcResult(expression, divide(expression));

        case 'H':
            return halt(expression);

        case 'C':
            return showCalcResult(expression, compare(expression));
        
        default:
            printf("Error: invalid opcode\n");
            return FALSE;
    }
}

int showCalcResult(OPERATOR *expression, int result) {
    // 1) check opcode
    // 2) check whether op1 is R or 0x
    printf("%s %s %s ", expression->opcode, expression->op1, expression->op2);

    if (strncmp(expression->opcode, "M", 1) == 0) {
        if (expression->op1[1] == 'R') {
            printf("=> %s: %d\n", expression->op1, result);
        } else {
            printf("=> R0: %d\n", result);
        }
        return TRUE;
    }

    if (strncmp(expression->opcode, "C", 1) == 0) {
        printf("=> R0: %d\n", result);
        return TRUE;
    }

    if (expression->op1[0] == 'R') {
        printf("=> %s: %d = %d %s %d\n",
            expression->op1, result, 
            expression->preValue, expression->opcode, (int)strtol(expression->op2, NULL, 16));
    } else {
        printf("=> R0: %d = %d %s %d\n", 
            result, 
            (int)strtol(expression->op1, NULL, 16), expression->opcode, (int)strtol(expression->op2, NULL, 16));
    }

    return TRUE;
}