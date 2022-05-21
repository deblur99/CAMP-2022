#pragma once

#ifndef DECODE_H
#define DECODE_H
#endif

#include "../utils/defines.h"

void decode(INSTRUCT *inst, u_int32_t target);

void decodeRType(INSTRUCT *inst, u_int32_t target);

void decodeIType(INSTRUCT *inst, u_int32_t target);

void decodeJType(INSTRUCT *inst, u_int32_t target);