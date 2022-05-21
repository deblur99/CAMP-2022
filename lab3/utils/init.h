#pragma once

#ifndef INIT_H
#define INIT_H
#endif

#include "defines.h"

u_int32_t* initMainMemory();

SCYCLE_HANDLER* initHandler();

u_int32_t* initRegMemory();

PC* initPC();

INSTRUCT* initInstruction();

COUNTER* initCounter();

void freeHandler(SCYCLE_HANDLER *handler);

void freePC(PC *PC);

void freeInstruction(INSTRUCT *inst);

void freeCounter(COUNTER *counter);