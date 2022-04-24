#pragma once

#ifndef EXECUTE_H
#define EXECUTE_H
#endif

#include "../utils/defines.h"

void execute(INSTRUCT *inst, u_int32_t regMemory[], u_int32_t *PC);

void executeRType(INSTRUCT *inst, u_int32_t regMemory[], u_int32_t *PC);

void executeIType(INSTRUCT *inst, u_int32_t regMemory[], u_int32_t *PC);

void executeJType(INSTRUCT *inst, u_int32_t regMemory[], u_int32_t *PC);