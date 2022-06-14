#pragma once

#ifndef INST_H
#define INST_H
#endif

#include <sys/types.h>

class Inst {
protected:
    char optype;
    u_int32_t opcode = 0x0;         // for R, I, J type

    u_int32_t rs = 0x0;             // for R, I type          
    u_int32_t rt = 0x0;             // for R, I type
    u_int32_t rd = 0x0;             // for R type

    u_int32_t shmat = 0x0;          // for R type
    u_int32_t funct = 0x0;          // for R type

    int16_t immed = 0x0;            // for I type

    int32_t signExtImm = 0x0;       // for I type (addi, addiu, lbu, lhu, lw, slti, sltiu, sb, sc, sh, sw, lwcu, ldcl, swcl, sdcl)
    int32_t zeroExtImm = 0x0;       // for I type (andi, ori)

    bool isMetBranchCond = false;   // for Branch (beq, bne)
    u_int32_t branchAddr = 0x0;     // for Branch (beq, bne)

    u_int32_t jumpAddr = 0x0;       // for Jump (j, jal)
};