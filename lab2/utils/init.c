// 초기화 및 메모리 해제 함수 정의
#include "init.h"

// 할당
MAIN_MEMORY* initMainMemory() {
    MAIN_MEMORY *mainMemory = (MAIN_MEMORY *)malloc(sizeof(MAIN_MEMORY));
    memset(mainMemory, 0x0, sizeof(MAIN_MEMORY));

    FILE *fp = fopen("/mnt/c/Users/32184893/CAMP-2022/lab2/test_prog/input4.bin", "rb");
    if (fp == NULL) {
        perror("File Not Found");
        exit(1);
    }

    int amount = 0; // amount of binary values in .o    
    while (fread(&mainMemory->MEMORY[4 * amount++], 1, sizeof(int), fp) == 4) {
        ;
    }

    printf("=========================\n");
    printf("All loaded data from .bin\n");
    printf("=========================\n");
    // print all loaded data from MEMORY array
    for (int i = 0; i < amount; i++) {
        printf("0x%X: 0x%08X\n", 4 * i, mainMemory->MEMORY[4 * i]);
    }
    printf("=========================\n");
    
    fclose(fp);

    mainMemory->endPoint = amount;

    return mainMemory;
}

SCYCLE_HANDLER* initHandler() {
    SCYCLE_HANDLER *handler = (SCYCLE_HANDLER *)malloc(sizeof(SCYCLE_HANDLER));
    memset(handler, 0, sizeof(SCYCLE_HANDLER));

    return handler;
}

int32_t* initRegMemory() {
    int32_t *regMemory = (int32_t *)malloc(sizeof(int32_t) * REG_MEMORY_SIZE);
    memset(regMemory, 0, REG_MEMORY_SIZE);

    return regMemory;
}

PC* initPC() {
    PC *pc = (PC *)malloc(sizeof(PC));
    return pc;
}

INSTRUCT* initInstruction() {
    INSTRUCT *inst = (INSTRUCT *)malloc(sizeof(INSTRUCT));
    memset(inst, 0, sizeof(INSTRUCT));

    return inst;
}

COUNTER* initCounter() {
    COUNTER *counter = (COUNTER *)malloc(sizeof(COUNTER));
    memset(counter, 0, sizeof(COUNTER));    

    return counter;
}

// 메모리 해제
void freeMainMemory(MAIN_MEMORY *memory) {
    free(memory);
}

void freeHandler(SCYCLE_HANDLER *handler) {
    free(handler);
}

void freeRegMemory(u_int32_t *regMemory) {
    free(regMemory);
}

void freePC(PC *PC) {
    free(PC);
}

void freeInstruction(INSTRUCT *inst) {
    free(inst);
}

void freeCounter(COUNTER *counter) {
    free(counter);
}