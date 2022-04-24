#pragma once

#ifndef INIT_H
#define INIT_H
#endif

#include "defines.h"

u_int32_t* initProgram();

u_int32_t* initPC();

INSTRUCT* initInstruction();

void freeMemory(u_int32_t *memory);

void freePC(u_int32_t *PC);

void freeInstruction(INSTRUCT *inst);