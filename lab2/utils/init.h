#pragma once

#ifndef INIT_H
#define INIT_H
#endif

#include "defines.h"

u_int32_t* initProgram();

INSTRUCT* initInstruction();

REGISTERS* initRegisters();

void freeMemory(u_int32_t *memory);

void freeInstruction(INSTRUCT *inst);

void freeRegisters(REGISTERS *registers);