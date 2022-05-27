#include <cstdio>
#include <sys/types.h>

void test1() {
    u_int32_t inst = 0x0c000000;

    int opcode;
    int addr;

    opcode = inst >> 26;
    inst = inst << 6 >> 6;

    addr = inst;

    printf("addr: 0x%08X\n", addr);
}

void test2() {
    int inst = 0x0D00000C;

    u_int32_t result =        0x00000000;
    u_int32_t r1     = inst & 0xFF000000;
    u_int32_t r2     = inst & 0x00FF0000;
    u_int32_t r3     = inst & 0x0000FF00;
    u_int32_t r4     = inst & 0x000000FF;

    result = result | (r1 >> 24);
    result = result | (r2 >> 8);
    result = result | (r3 << 8);
    result = result | (r4 << 24);

    inst = result;

    printf("0x%08X\n", inst);
}

void test3() {
    int inst = 0x27bdfff8;

    // opcode
    int opcode = inst >> 26;
    inst = inst & 0x3FFFFFF; // strip opcode
    
    // rs
    int rs = inst >> 21;
    inst = inst & 0x1FFFFF; // strip rs

    // rt
    int rt = inst >> 16;
    inst = inst & 0xFFFF; // strip rt

    // immediate
    int immed = (int16_t)(inst);

    // signExtImm
    u_int32_t MSB = immed >> 15;
    u_int32_t result = 0x00000000;

        result = result | inst;
        for (int i = 0; i < 16; i++) {
            result  = result | MSB;
            MSB = MSB << 1;
        }
        int signExtImm = result;

        result = 0x0; // init

        // ZeroExtImm
        result = (u_int32_t)(immed) | 0x00000000;
        int zeroExtImm = result;

    printf("rs: 0x%08X, rt: 0x%08X\n", rs, rt);
}

void test4() {
    u_int32_t PC = 0x50;
    int32_t a = 0xFFFFFFF3;
    printf("a: %d\n", a << 2);

    PC = (PC + 4) + (a << 2);

    // 왼쪽으로 짤린 비트는 오버플로가 발생하지만,
    // 오른쪽으로 짤린 비트는 아예 소멸된다.
    printf("a: 0x%08X %d\n", PC, PC);
}

int main() {
    test4();
    
    return 0;
}