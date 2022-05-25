#include <cstdio>

int main() {
    int inst = 0x1440fff3;

    int opcode = inst >> 26;
    inst = inst << 6 >> 6;

    // rs
    int rs = inst >> 21;
    inst = inst << 11 >> 11; // strip rs

        // rt
    int rt = inst >> 16;
    inst = inst << 16 >> 16; // strip rt

    // immediate
    int immed = (short)inst;

    // signExtImm
    int MSB = immed >> 15 << 16;
    int result = 0x00000000;

    result = result | inst;
    for (int i = 0; i < 16; i++) {
        result  = result | MSB;
        MSB = MSB << 1;
    }
    int signExtImm = result;

    result = 0x0; // init

    // ZeroExtImm
    result = (immed) | 0x00000000;
    int zeroExtImm = result;
    
    printf("rs: 0x%08X, rt: 0x%08X, immed: 0x%04X\n", rs, rt, immed);
    
    return 0;
}