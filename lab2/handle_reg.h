#pragma once

#ifndef HANDLE_REG
#define HANDLE_REG
#endif

#include "defines.h"

__uint32_t updatePC(__uint32_t PC, INSTRUCT *inst, REGISTERS *regs);