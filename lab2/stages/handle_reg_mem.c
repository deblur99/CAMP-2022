#include "handle_reg_mem.h"

u_int32_t updatePC(INSTRUCT *inst, REGISTERS *regs) {
    regs->PC += 4;
    return regs->PC;
}