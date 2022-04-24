#include "fetch.h"

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

    // debug
    printf("0x%08X\n", result);

    return result;
}

__uint32_t fetch(int PC, __uint32_t *MEMORY) {
    if (PC >= MEMORY_SIZE) {
        return -1;
    }

    return convertFromBigEToLittleE(MEMORY[PC]);    
}