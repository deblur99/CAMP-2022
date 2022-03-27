#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE                    1
#define FALSE                   0

#define R0_POINT                0
#define R1_POINT                1
#define R2_POINT                2
#define R3_POINT                3
#define R4_POINT                4
#define R5_POINT                5
#define R6_POINT                6
#define R7_POINT                7
#define R8_POINT                8
#define R9_POINT                9

#define AMOUNT_REGS             10
#define REG_INIT_VAL            0x0

#define BUF_LENGTH              200
#define BUF_LETTER_LENGTH       2

#define IS_REGISTER_TOKEN       0   // first letter of register is 'R'
#define REGISTER_NUMBER         1   
#define IS_REGISTER             2   // length of register is 2