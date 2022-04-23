/*
4/16 한 일
- 파서 제작
 - 바이너리 파일에서 명령어 한 줄씩 읽어와서, 각 명령어를 리틀 엔디안 구조로 변환한 후 정적 메모리에 저장함
 - 가용 메모리가 충분치 않아 가급적 구동은 Assam 서버에서 하는 걸 추천
 - 현재는 메모리 크기를 0x10000로 잡았지만, 실제 제출용 코드에서는 0xFFFFFFFF로 잡아야 함

이후 해야할 일 (제출마감 4/24 일요일. Freeday는 5일 있음)
1) Decode
2) Execute
3) Write memory
4) Write register
5) Update PC
6) 매 명령어 실행 끝날 때마다 레지스터의 현재 state, 사용한 명령어의 개수 등등 출력
*/

#include "defines.h"

OPERATOR *operator;
REGISTERS *regs;

__uint32_t MEMORY[MEMORY_SIZE] = {-1, };    // instructions of .o file will be saved in there
__uint32_t PC = 0;                      // when PC points 0xFFFFFFFF, then terminate the program.
static __uint32_t inst = 0x0;

static int amount = 0; // amount of binary values in .o

void initProgram() {  
    FILE *fp = fopen("/mnt/c/Users/32184893/CAMP-2022/lab2/test_prog/simple.o", "rb");
    if (fp == NULL) {
        perror("File Not Found");
        exit(1);
    }

    while (!feof(fp)) {
        fread(&MEMORY[4 * amount++], sizeof(int), 1, fp);
    }
    
    fclose(fp);

    PC = 0x0; // initialize PC to front of MEMORY

    operator = (OPERATOR *)malloc(sizeof(OPERATOR));
    memset(operator, 0, sizeof(operator));

    regs = (REGISTERS *)malloc(sizeof(REGISTERS));
    memset(regs, 0, sizeof(REGISTERS));
    regs->sp = 0x10000000;
    regs->ra = 0xFFFFFFFF;
}

void freeMemory() {
    free(operator);
    free(regs);
}

__uint32_t convertFromBigEToLittleE(__uint32_t target) {
    __uint32_t result =          0x00000000;

    __uint32_t r1     = target & 0xFF000000;
    __uint32_t r2     = target & 0x00FF0000;
    __uint32_t r3     = target & 0x0000FF00;
    __uint32_t r4     = target & 0x000000FF;

    result = result | (r1 >> 24);
    result = result | (r2 >> 8);
    result = result | (r3 << 8);
    result = result | (r4 << 24);

    return result;
}

void fetch() {
    // if PC reaches threshold, then halt the program.
    if (PC >= MEMORY_SIZE) {
        return;
    }
    inst = convertFromBigEToLittleE(MEMORY[PC]);
    PC += 0x4; // 나중에 마지막 단계까지 구현하면 마지막 단계로 옮기기
}

void decode() {
    // get opcode
    operator->opcode = inst >> 26;

    printf("0x%08X 0x%08X\n", inst, operator->opcode); // debug
    
    switch (operator->opcode) {
    case 0x27:
        
        break;
    
    default:
        break;
    }

    // while (!feof(fp)) {
    //     while (!feof(fp) && ((inst << 24) != ADDI)) {
    //         fread(&inst, sizeof(int), 1, fp);
    //     }

    //     // starting point
    //     if (!feof(fp))
    //         MEMORY[currentIdx++] = convertFromBigEToLittleE(inst);

    //     while (!feof(fp)) {
    //         fread(&inst, sizeof(int), 1, fp);

    //         // actual instructions are between addiu (27bd~) and jr (03e0~)
    //         if ((inst << 16) == JR) {
    //             MEMORY[currentIdx++] = convertFromBigEToLittleE(inst);
    //             break;
    //         }
    //         MEMORY[currentIdx++] = convertFromBigEToLittleE(inst);
    //     }
    // }
}

void show() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (MEMORY[i] != 0) {
            printf("0x%08X\n", MEMORY[i]);
        }
    }
}

void showCurrentStatus() {
    
}

int main(int argc, char *argv[]) {
    // bring all binary codes from .o file
    initProgram();

    for (int i = 0; i < amount; i++) {
        fetch();
        decode();
    }
        
    showCurrentStatus();

    //show();

    freeMemory();
    return 0;
}