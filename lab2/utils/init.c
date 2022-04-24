// 초기화 및 메모리 해제 함수 정의
#include "init.h"

// 할당
u_int32_t* initProgram() {
    u_int32_t *MEMORY = (u_int32_t *)malloc(sizeof(u_int32_t) * MEMORY_SIZE);
    memset(MEMORY, -1, MEMORY_SIZE);

    FILE *fp = fopen("/mnt/c/Users/32184893/CAMP-2022/lab2/test_prog/simple.o", "rb");
    if (fp == NULL) {
        perror("File Not Found");
        exit(1);
    }

    int amount = 0; // amount of binary values in .o    
    while (!feof(fp)) {
        fread(&MEMORY[4 * amount++], sizeof(int), 1, fp);
    }
    
    fclose(fp);

    return MEMORY;
}

INSTRUCT* initInstruction() {
    INSTRUCT *inst = (INSTRUCT *)malloc(sizeof(INSTRUCT));
    memset(inst, 0, sizeof(INSTRUCT));

    return inst;
}

REGISTERS* initRegisters() {
    REGISTERS *regs = (REGISTERS *)malloc(sizeof(REGISTERS));
    memset(regs, 0, sizeof(REGISTERS));
    regs->sp = 0x10000000;
    regs->ra = 0xFFFFFFFF;

    return regs;
}

// 초기화
void freeMemory(u_int32_t *memory) {
    free(memory);
}

void freeInstruction(INSTRUCT *inst) {
    free(inst);
}

void freeRegisters(REGISTERS *registers) {
    free(registers);
}