#include "handle_reg.h"

__uint32_t updatePC(__uint32_t PC, INSTRUCT *inst, REGISTERS *regs) {
    PC += 4;
    return PC;
}