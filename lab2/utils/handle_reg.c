#include "handle_reg.h"

u_int32_t updatePC(u_int32_t PC, INSTRUCT *inst, REGISTERS *regs) {
    PC += 4;
    return PC;
}