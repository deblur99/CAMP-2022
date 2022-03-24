#include "operators.h"

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

        // TO DO : process command from "stringArr"
        
    }

    free(letter);
    free(lineString);
    for (int i = 0; i < AMOUNT_REGS; i++) {
        free(stringArr[i]);
    }
    fclose(fp);

    return 0;
}