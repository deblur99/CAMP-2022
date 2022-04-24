// fetch.h depends on handle_reg.h
#pragma once

#ifndef FETCH_H
#define FETCH_H
#endif

#include "../utils/defines.h"

u_int32_t convertFromBigEToLittleE(u_int32_t target);

u_int32_t fetch(u_int32_t *PC, u_int32_t *MEMORY);