#pragma once

#ifndef DEFINE_H
#define DEFINE_H
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE     0x10000 // original size is 0xFFFFFFFF

#define OPCODE_MASK     0x000000FF

#define ADDI            0x27000000
#define JR              0xE0030000

typedef struct _INSTRUCTOR {
    
}INST;