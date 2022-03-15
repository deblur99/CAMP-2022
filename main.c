#include "operators.h"

// registers (R0 ~  R9; as global variables)
// initialize their values into 0x0
extern int registers[AMOUNT_REGS];

static int *R0;
static int *R1;
static int *R2;
static int *R3;
static int *R4;
static int *R5;
static int *R6;
static int *R7;
static int *R8;
static int *R9;

void initRegisters() {
    for (int i = 0; i < AMOUNT_REGS; i++) {
        registers[i] = REG_INIT_VAL;
    }

    // assign R0~9 registers to address of each element of static array
    R0 = registers;
    R1 = registers + 1;
    R2 = registers + 2;
    R3 = registers + 3;
    R4 = registers + 4;
    R5 = registers + 5;
    R6 = registers + 6;
    R7 = registers + 7;
    R8 = registers + 8;
    R9 = registers + 9;
}

int main(int argc, char *argv[]) {
    // initialize
    initRegisters();

    FILE *fp = fopen("./myinput.txt", "r");
    char *buf = (char *)malloc(sizeof(char) * BUF_LENGTH);

    char **procBuf = (char **)malloc(sizeof(char *) * BUF_LENGTH);

    for (int i = 0; i < BUF_LENGTH; i++) {
        procBuf[i] = (char *)malloc(sizeof(char) * BUF_LENGTH);
    }

    // debug
    fseek(fp, 0, SEEK_END);
    printf("length of input.txt : %d\n", (int)(ftell(fp)));
    fseek(fp, 0, SEEK_SET);

    // get string from input.txt
    while (feof(fp) != TRUE)
        procBuf = getOneOperationFromTxt(fp, procBuf);
    
    // debug
    // for (int i = 0; i < BUF_LENGTH; i++) {
    //     printf("%s ", procBuf[i]);
    // }
    // printf("\n");

    // printf("%c %c %c\n", procBuf[0], procBuf[1], procBuf[2]);    

    // process the string
    

    // free dynamic memories and file pointer
    for (int i = 0; i < BUF_LENGTH; i++) {
        free(procBuf[i]);
    }

    free(procBuf);
    free(buf);

    fclose(fp);

    return 0;
}

