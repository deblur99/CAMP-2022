// included:
// (step 4) Access memory, (step 5) Writeback, Update PC
#pragma once

#ifndef HANDLE_REG
#define HANDLE_REG
#endif

#include "../utils/defines.h"

u_int32_t updatePC(INSTRUCT *inst, REGISTERS *regs);