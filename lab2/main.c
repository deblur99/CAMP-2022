#include "defines.h"

static __uint32_t MEMORY[MEMORY_SIZE] = {0x0, };
__uint32_t *PC = NULL; // when PC points 0xFFFFFFFF, then terminate the program.

__uint32_t convertFromBigToLittle(__uint32_t target) {
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
    __uint32_t inst = 0x0;
    int currentIdx = 0;

    FILE *fp = fopen("lab2/test_prog/simple.o", "rb");
    if (fp == NULL) {
        perror("File Not Found");
        exit(1);
    }

    while (!feof(fp) && ((inst & ADDI) != ADDI)) {
        fread(&inst, sizeof(int), 1, fp);
    }
    
    // starting point
    printf("0x%08X\n", convertFromBigToLittle(inst));

    while (!feof(fp)) {
        fread(&inst, sizeof(int), 1, fp);

        // actual instructions are between addiu (27bd~) and jr (03e0~)
        if ((inst & JR) == JR) {
            printf("0x%08X\n", convertFromBigToLittle(inst));
            break;
        }
        printf("0x%08X\n", convertFromBigToLittle(inst));
    }

    fclose(fp);
}


int main(int argc, char *argv[]) {
    fetch();

    return 0;
}