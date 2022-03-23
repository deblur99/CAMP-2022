#include "operators.h"

extern int registers[AMOUNT_REGS];

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

    // old
    // map R0~9 registers to address of each element of static array
    // R0 = registers;
    // R1 = registers + 1;
    // R2 = registers + 2;
    // R3 = registers + 3;
    // R4 = registers + 4;
    // R5 = registers + 5;
    // R6 = registers + 6;
    // R7 = registers + 7;
    // R8 = registers + 8;
    // R9 = registers + 9;

    // debug
    for (int i = 0; i < AMOUNT_REGS; i++)
        printf("%d ", *registers + i);
}

int main(int argc, char *argv[]) {
    // initialize
    initRegisters();

    FILE *fp = fopen("./myinput.txt", "r");
    char **stringArr = (char **)malloc(sizeof(char *) * BUF_LENGTH);

    for (int i = 0; i < BUF_LENGTH; i++) {
        stringArr[i] = (char *)malloc(sizeof(char) * BUF_LENGTH);
    }

    // debug (below 3 lines)
    fseek(fp, 0, SEEK_END);
    printf("length of input.txt : %d\n", (int)(ftell(fp)));
    fseek(fp, 0, SEEK_SET);

    // get string from input.txt
    while (feof(fp) != TRUE)
        stringArr = getOneOperationFromTxt(fp, stringArr);


    // TO DO : process the string
    

    // free both dynamic memory and file pointer
    for (int i = 0; i < BUF_LENGTH; i++) {
        free(stringArr[i]);
    }

    free(stringArr);

    fclose(fp);

    return 0;
}