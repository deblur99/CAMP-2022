#pragma once

#ifndef MEM_ACCESS_H
#define MEM_ACCESS_H
#endif

#include "../utils/defines.h"

int accessMemory(u_int32_t opcode);

void readFromMemory(MAIN_MEMORY *mainMemory, SCYCLE_HANDLER *handler);

void writeIntoMemory(MAIN_MEMORY *mainMemory, SCYCLE_HANDLER *handler);