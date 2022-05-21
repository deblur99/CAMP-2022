#pragma once

#ifndef EXECUTE_H
#define EXECUTE_H
#endif

#include "../utils/defines.h"

void execute(SCYCLE_HANDLER *handler, MAIN_MEMORY *mainMemory);

void executeRType(SCYCLE_HANDLER *handler);

void executeIType(SCYCLE_HANDLER *handler, MAIN_MEMORY *mainMemory);

void executeJType(SCYCLE_HANDLER *handler);