#include "defines.h"

void initRegisters() {
    for (int i = 0; i < AMOUNT_REGS; i++) {
        registers[i] = REG_INIT_VAL;
    }
}