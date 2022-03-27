#include "defines.h"
#include "operators.h"

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

int main(int argc, char *argv[]) {
    initRegisters();

    FILE *fp = fopen("myinput.txt", "r");
    if (fp == NULL) {
        perror("File Not Found");
        return -1;
    }

    char *stringArr[AMOUNT_REGS] = { NULL, }; // for saving tokens of tokinized string
    for (int i = 0; i < AMOUNT_REGS; i++) {
        stringArr[i] = (char *)malloc(sizeof(char) * AMOUNT_REGS);
    }
    char *letter = (char *)malloc(sizeof(char) * BUF_LETTER_LENGTH); // for saving one letter of fullString
    char *fullString = (char *)malloc(sizeof(char) * BUF_LENGTH); // for saving one line of input.txt

    while (feof(fp) != TRUE) {
        memset(letter, '\0', BUF_LETTER_LENGTH);
        memset(fullString, '\0', BUF_LENGTH);

        while (fread(letter, sizeof(char), BUF_LETTER_LENGTH - 1, fp) &&
            feof(fp) != TRUE &&
            strcmp(letter, "\n") != 0) {

            if (fullString[0] == '\0') {
                strcpy(fullString, letter);
            } else {
                strcat(fullString, letter);
            }
        }

        int idx = 0;
        char *tok = (char *)malloc(sizeof(char) * BUF_LENGTH);

        tok = strtok(fullString, " ");
        strcpy(stringArr[idx++], tok);

        while (tok != NULL) {
            tok = strtok(NULL, " ");
            if (tok != NULL)
                strcpy(stringArr[idx++], tok);
        }

        // TO DO : parse string elements of stringArr
        // check string denotes register or value
        int target1 = 0, target2 = 0;
        if (stringArr[OPERAND1][IS_REGISTER_TOKEN] == 'R' &&
            strlen(stringArr[OPERAND1]) == IS_REGISTER) {

            if (isdigit(stringArr[OPERAND1][REGISTER_NUMBER])) {
                switch (stringArr[OPERAND1][REGISTER_NUMBER]) {
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
        }

        if (stringArr[OPERAND2][IS_REGISTER_TOKEN] == 'R' &&
            strlen(stringArr[OPERAND2]) == IS_REGISTER) {

            if (isdigit(stringArr[OPERAND2][REGISTER_NUMBER])) {
                switch (stringArr[OPERAND2][REGISTER_NUMBER]) {
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
    }

    // after all tasks end, terminate program
    free(fullString);
    free(letter);
    for (int i = 0; i < AMOUNT_REGS; i++) {
        free(stringArr[i]);
    }
    fclose(fp);

    return 0;
}