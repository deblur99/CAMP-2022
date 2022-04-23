#pragma once

#ifndef INIT_H
#define INIT_H
#endif

#include "defines.h"

__uint32_t* initProgram();

INSTRUCT* initInstruction();

REGISTERS* initRegisters();

void freeMemory(__uint32_t *memory);

void freeInstruction(INSTRUCT *inst);

void freeRegisters(REGISTERS *registers);