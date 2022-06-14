#pragma once

#ifndef COUNTER_H
#define COUNTER_H
#endif

#include <sys/types.h>

class Counter {
protected:
    int32_t returnValue = 0;

    u_int32_t executedInst = 0;
    u_int32_t executedRTypeInst = 0;
    u_int32_t executedITypeInst = 0;
    u_int32_t executedJTypeInst = 0;
    u_int32_t memoryAccessInst = 0;

    u_int32_t takenBranches = 0;
};