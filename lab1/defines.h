#pragma once

#ifndef _DEFINES_H
#define _DEFINES_H
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE                    1
#define FALSE                   0

#define AMOUNT_REGS             10
#define REG_INIT_VAL            0x0

#define BUF_LENGTH              200
#define BUF_LETTER_LENGTH       2

#define OP_LENGTH               10

// register
static int registers[AMOUNT_REGS];

void initRegisters();

// operator format
typedef struct _OPERATOR {
    char opcode[OP_LENGTH];
    char op1[OP_LENGTH];
    char op2[OP_LENGTH];
    int preValue; // the op1 value before calculating
}OPERATOR;