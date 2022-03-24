#include "operators.h"
#include <stdbool.h>

enum _TARGET {
    OPCODE = 0,
    OPERAND1,
    OPERAND2
};

enum _USING_REGISTER {
    REG0 = 0,
    REG1, REG2, REG3, REG4, REG5, REG6, REG7, REG8, REG9
};

static int registers[AMOUNT_REGS];

// registers (R0 ~  R9; as global variables)
// initialize their values into 0x0
const static int *R0 = registers;
const static int *R1 = registers + 1;
const static int *R2 = registers + 2;
const static int *R3 = registers + 3;
const static int *R4 = registers + 4;
const static int *R5 = registers + 5;
const static int *R6 = registers + 6;
const static int *R7 = registers + 7;
const static int *R8 = registers + 8;
const static int *R9 = registers + 9;

void initRegisters() {
    for (int i = 0; i < AMOUNT_REGS; i++) {
        registers[i] = REG_INIT_VAL;
    }
}

int main(int argc, char *argv[]) {
    initRegisters();

    FILE *fp = fopen("myinput.txt", "r");
    if (fp == NULL) {
        perror("File Not Found");
        return -1;
    }

    char *stringArr[AMOUNT_REGS] = { NULL, };
    for (int i = 0; i < AMOUNT_REGS; i++) {
        stringArr[i] = (char *)malloc(sizeof(char) * AMOUNT_REGS);
    }
    char *lineString = (char *)malloc(sizeof(char) * BUF_LENGTH);
    char *letter = (char *)malloc(sizeof(char) * 2);

    // get string from input.txt
    while (feof(fp) != TRUE) {
        memset(lineString, '\0', BUF_LENGTH);
        memset(letter, '\0', 2);

        // get one line from input.txt
        while (fread(letter, sizeof(char), 1, fp) &&
            feof(fp) != TRUE &&
            strcmp(letter, "\n") != 0) {

            if (lineString[0] == '\0') {
                strcpy(lineString, letter);
            } else {
                strcat(lineString, letter);
            }
        }

        // tokenize the gotten line
        int idx = 0;
        char *tok = (char *)malloc(sizeof(char) * BUF_LENGTH);

        tok = strtok(lineString, " ");
        strcpy(stringArr[idx++], tok);

        while (tok != NULL) {
            tok = strtok(NULL, " ");
            if (tok != NULL)
                strcpy(stringArr[idx++], tok);
        }
        

        // save register number to be used
        int target1 = 0, target2 = 0;

        // TO DO : parse string elements of stringArr
        // check string denotes register or value
        if (stringArr[OPERAND1][0] == 'R' && strlen(stringArr[OPERAND1] == 2)) {
            if (isdigit(stringArr[OPERAND1][1])) {
                switch (stringArr[OPERAND1][1]) {
                case '0':
                    target1 = REG0;
                    break;
                
                case '1':
                    target1 = REG1;
                    break;
                
                case '2':
                    target1 = REG2;
                    break;
                    
                case '3':
                    target1 = REG3;
                    break;
                
                case '4':
                    target1 = REG4;
                    break;

                case '5':
                    target1 = REG5;
                    break;

                case '6':
                    target1 = REG6;
                    break;

                case '7':
                    target1 = REG7;
                    break;

                case '8':
                    target1 = REG8;
                    break;

                case '9':
                    target1 = REG9;
                    break;
            }
        }

        if (stringArr[OPERAND2][0] == 'R' && strlen(stringArr[OPERAND2] == 2)) {
            if (isdigit(stringArr[OPERAND2][1])) {
                switch (stringArr[OPERAND2][1]) {
                case '0':
                    target2 = REG0;
                    break;
                
                case '1':
                    target2 = REG1;
                    break;
                
                case '2':
                    target2 = REG2;
                    break;
                    
                case '3':
                    target2 = REG3;
                    break;
                
                case '4':
                    target2 = REG4;
                    break;

                case '5':
                    target2 = REG5;
                    break;

                case '6':
                    target2 = REG6;
                    break;

                case '7':
                    target2 = REG7;
                    break;

                case '8':
                    target2 = REG8;
                    break;

                case '9':
                    target2 = REG9;
                    break;
                }
            }
        }        

        // TO DO : process command from "stringArr"
        int operand1 = 0, operand2 = 0;

        operand1 = atoi(stringArr[OPERAND1]);
        operand2 = atoi(stringArr[OPERAND2]);

        switch (stringArr[OPCODE][0]) {
        case 'M':
            mov(operand1, operand2);     
        case '+':
            add(operand1, operand2);
        case '-':
            sub(operand1, operand2);
        case '*':
            multiply(operand1, operand2);
        case '/':
            divide(operand1, operand2);
        }
    }

    free(letter);
    free(lineString);
    for (int i = 0; i < AMOUNT_REGS; i++) {
        free(stringArr[i]);
    }
    fclose(fp);

    return 0;
}