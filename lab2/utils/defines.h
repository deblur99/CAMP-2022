#pragma once

#ifndef DEFINE_H
#define DEFINE_H
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MEMORY_SIZE     0x10000 // original size is 0xFFFFFFFF
#define REG_MEMORY_SIZE 0x20       // values in each $0~$31 registers

#define OPCODE_MASK     0x000000FF

// opcode list (from MIPS Green Sheet)
// nop
#define EMPTY           0x00000000

// R type OPCODE is 0 (except MFC0. its opcode is 0x10)
#define RTYPE           0x0

// R type funct
#define MOVE            0x21

#define ADD             0x20 
#define ADDU            0x21
#define AND             0x24
#define JR              0x08
#define NOR             0x27
#define OR              0x25
#define SLT             0x2A
#define SLTU            0x2B
#define SLL             0x00
#define SRL             0x02
#define SUB             0x22
#define SUBU            0x23
#define DIV             0x1A
#define DIVU            0x1B
#define MFHI            0x10
#define MFLO            0x12
#define MFC0            0x10    // (except) OPCODE is 0x10. funct is 0x0
#define MULT            0x18
#define MULTU           0x19
#define SRA             0x3

// I type opcode
#define LI              0x24

#define ADDI            0x8
#define ADDIU           0x9
#define ANDI            0xC
#define BEQ             0x4
#define BNE             0x5
#define LBU             0x24
#define LHU             0x25
#define LL              0x30
#define LUI             0xF
#define LW              0x23
#define ORI             0xD
#define SLTI            0xA
#define SLTIU           0xB
#define SB              0x28
#define SC              0x38
#define SH              0x29
#define SW              0x2B
#define LWCL            0x31
#define LDCL            0x35
#define SWCL            0x39
#define SDCL            0x3D

// J type opcode
#define J               0x2
#define JAL             0x3

// FI, FR are skipped

// register list ($0 ~ $31, 0x00 ~ 0x1F)
typedef enum _REG_LIST {
    zero = 0x0,                   // always zero ($0)
    at = 0x1,                     // reserved for assembler ($1)
    
    // 1st, 2nd return value registers ($2, $3)
    v0 = 0x2, v1,                 

    // argument registers for function ($4 ~ $7)
    a0 = 0x4, a1, a2, a3,         

    // temporary registers ($8 ~ $15)
    t0 = 0x8, t1, t2, t3, t4, t5, t6, t7,

    // saved temporary registers ($16 ~ $23)
    s0 = 0x10, s1, s2, s3, s4, s5, s6, s7,

    // temporary registers ($24, $25)
    t8 = 0x18, t9,          

    // reserved for kernel ($26, $27)
    k0 = 0x1A, k1,

    gp = 0x1C,                      // global pointer ($28). 0x1C
    sp = 0x1D,                      // stack pointer ($29). 0x1D
    fp = 0x1E,                      // frame pointer ($30). 0x1E
    ra = 0x1F,                      // return address ($31). 0x1F

}REG_LIST;

// Handlers : REG_MEMORY, PC, INSTRUCT, COUNTER

// PCs
typedef struct _PC {
    u_int32_t prevPC;
    u_int32_t currPC;

}PC;

// structures
typedef struct _INSTRUCT {
    char optype[2];
    u_int32_t opcode;      // for R, I, J type

    u_int32_t rs;          // for R, I type          
    u_int32_t rt;          // for R, I type
    u_int32_t rd;          // for R type

    u_int32_t shmat;       // for R type
    u_int32_t funct;       // for R type

    int16_t immed;       // for I type
    
    u_int32_t address;     // for J type

    int32_t signExtImm;  // for I type (addi, addiu, lbu, lhu, lw, slti, sltiu, sb, sc, sh, sw, lwcu, ldcl, swcl, sdcl)
    int32_t zeroExtImm;  // for I type (andi, ori)

    u_int32_t branchAddr;  // for Branch (beq, bne)
    u_int32_t jumpAddr;    // for Jump (j, jal)

}INSTRUCT;

// for counting instructions, branches
typedef struct _COUNTER {
    int32_t returnValue;

    u_int32_t executedInst;
    u_int32_t executedRTypeInst;
    u_int32_t executedITypeInst;
    u_int32_t executedJTypeInst;
    u_int32_t memoryAccessInst;

    u_int32_t takenBranches;

}COUNTER;

typedef struct _SCYCLE_HANDLER {
    int32_t *regMemory;
    PC *PC;
    INSTRUCT *inst;
    COUNTER *counter;

}SCYCLE_HANDLER;