#pragma once

#ifndef FETCH_H
#define FETCH_H
#endif

#include "defines.h"

__uint32_t convertFromBigEToLittleE(__uint32_t target);

__uint32_t fetch(int PC, __uint32_t *MEMORY);