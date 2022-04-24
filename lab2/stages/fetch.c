#include "fetch.h"

u_int32_t convertFromBigEToLittleE(u_int32_t target) {
    u_int32_t result =          0x00000000;

    u_int32_t r1     = target & 0xFF000000;
    u_int32_t r2     = target & 0x00FF0000;
    u_int32_t r3     = target & 0x0000FF00;
    u_int32_t r4     = target & 0x000000FF;

    result = result | (r1 >> 24);
    result = result | (r2 >> 8);
    result = result | (r3 << 8);
    result = result | (r4 << 24);

    // debug
    printf("0x%08X\n", result);

    return result;
}

u_int32_t fetch(u_int32_t *PC, u_int32_t *MEMORY) {
    if (*PC >= MEMORY_SIZE) {
        return -1;
    }

    return convertFromBigEToLittleE(MEMORY[*PC]);    
}