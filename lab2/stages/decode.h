#pragma once

#ifndef DECODE_H
#define DECODE_H
#endif

#include "../utils/defines.h"

INSTRUCT* decode(INSTRUCT *inst, __uint32_t target);

INSTRUCT* decodeRType(INSTRUCT *inst, __uint32_t target);

INSTRUCT* decodeIType(INSTRUCT *inst, __uint32_t target);

INSTRUCT* decodeJType(INSTRUCT *inst, __uint32_t target);