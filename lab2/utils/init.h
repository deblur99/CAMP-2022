#pragma once

#ifndef INIT_H
#define INIT_H
#endif

#include "defines.h"

MAIN_MEMORY* initMainMemory();

SCYCLE_HANDLER* initHandler();

int32_t* initRegMemory();

PC* initPC();

INSTRUCT* initInstruction();

COUNTER* initCounter();

void freeMainMemory(MAIN_MEMORY *memory);

void freeHandler(SCYCLE_HANDLER *handler);

void freeRegMemory(u_int32_t *regMemory);

void freePC(PC *PC);

void freeInstruction(INSTRUCT *inst);

void freeCounter(COUNTER *counter);