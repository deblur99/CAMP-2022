// 초기화 및 메모리 해제 함수 정의
#include "init.h"

// 할당
u_int32_t* initMainMemory() {
    u_int32_t *MEMORY = (u_int32_t *)malloc(sizeof(u_int32_t) * MEMORY_SIZE);
    memset(MEMORY, 0, MEMORY_SIZE);

    FILE *fp = fopen("/home/hyeonmin18/CAMP-2022/lab2/test_prog/input4.bin", "rb");
    if (fp == NULL) {
        perror("File Not Found");
        exit(1);
    }

    int amount = 0; // amount of binary values in .o    
    while (!feof(fp)) {
        fread(&MEMORY[4 * amount++], sizeof(int), 1, fp);
    }

    printf("=========================\n");
    printf("All loaded data from .bin\n");
    printf("=========================\n");
    // print all loaded data from MEMORY array
    for (int i = 0; i < amount; i++) {
        printf("0x%X: 0x%08X\n", 4 * i, MEMORY[4 * i]);
    }
    printf("=========================\n\n");
    
    fclose(fp);

    return MEMORY;
}

SCYCLE_HANDLER* initHandler() {
    SCYCLE_HANDLER *handler = (SCYCLE_HANDLER *)malloc(sizeof(SCYCLE_HANDLER));
    memset(handler, 0, sizeof(SCYCLE_HANDLER));

    return handler;
}

int32_t* initRegMemory() {
    int32_t *regMemory = (int32_t *)malloc(sizeof(int32_t) * REG_MEMORY_SIZE);
    memset(regMemory, 0, sizeof(regMemory));

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
void freeMainMemory(u_int32_t *memory) {
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