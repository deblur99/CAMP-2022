#pragma once

#ifndef EXECUTE_H
#define EXECUTE_H
#endif

#include "../utils/defines.h"
#include "handle_reg_mem.h"

REGISTERS* execute(INSTRUCT *inst, REGISTERS *regs, u_int32_t regMemory[]);

REGISTERS* executeRType(INSTRUCT *inst, REGISTERS *regs, u_int32_t regMemory[]);

REGISTERS* executeIType(INSTRUCT *inst, REGISTERS *regs, u_int32_t regMemory[]);

REGISTERS* executeJType(INSTRUCT *inst, REGISTERS *regs);