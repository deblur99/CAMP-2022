// 초기화 및 메모리 해제 함수 정의
#include "init.h"

// 할당
u_int32_t* initProgram() {
    u_int32_t *MEMORY = (u_int32_t *)malloc(sizeof(u_int32_t) * MEMORY_SIZE);
    memset(MEMORY, -1, MEMORY_SIZE);

    FILE *fp = fopen("/mnt/c/Users/deblu/CAMP/lab2/test_prog/simple.bin", "rb");
    if (fp == NULL) {
        perror("File Not Found");
        exit(1);
    }

    int amount = 0; // amount of binary values in .o    
    while (!feof(fp)) {
        fread(&MEMORY[4 * amount++], sizeof(int), 1, fp);
    }

    // debug
    for (int i = 0; i < amount; i++) {
        printf("0x%08X\n", MEMORY[4 * i]);
    }
    
    fclose(fp);

    return MEMORY;
}

u_int32_t* initPC() {
    u_int32_t *PC = (u_int32_t *)malloc(sizeof(u_int32_t));
    return PC;
}

INSTRUCT* initInstruction() {
    INSTRUCT *inst = (INSTRUCT *)malloc(sizeof(INSTRUCT));
    memset(inst, 0, sizeof(INSTRUCT));

    return inst;
}

// 초기화
void freeMemory(u_int32_t *memory) {
    free(memory);
}

void freePC(u_int32_t *PC) {
    free(PC);
}

void freeInstruction(INSTRUCT *inst) {
    free(inst);
}
