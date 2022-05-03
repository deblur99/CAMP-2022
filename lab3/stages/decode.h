#pragma once

#ifndef DECODE_H
#define DECODE_H
#endif

#include "../utils/defines.h"

INSTRUCT* decode(INSTRUCT *inst, u_int32_t target);

INSTRUCT* decodeRType(INSTRUCT *inst, u_int32_t target);

INSTRUCT* decodeIType(INSTRUCT *inst, u_int32_t target);

INSTRUCT* decodeJType(INSTRUCT *inst, u_int32_t target);