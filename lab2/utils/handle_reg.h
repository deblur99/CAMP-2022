#pragma once

#ifndef HANDLE_REG
#define HANDLE_REG
#endif

#include "defines.h"

u_int32_t updatePC(u_int32_t PC, INSTRUCT *inst, REGISTERS *regs);