#pragma once

#ifndef DEFINE_H
#define DEFINE_H
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE     0x10000 // original size is 0xFFFFFFFF

#define OPCODE_MASK     0x000000FF

// opcode list (from MIPS Green Sheet)
// R type funct
#define ADD             0x0 
#define ADDU            0x0
#define AND             0x0
#define JR              0x08
#define NOR             0x27
#define OR              0x25
#define SLT             0x2A
#define SLTU            0x2B
#define SLL             0x00
#define SLR             0x02
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

// structures
typedef struct _OPERATOR {
    char optype[2];
    __uint32_t opcode;      // for R, I, J type

    __uint32_t rs;          // for R, I type          
    __uint32_t rt;          // for R, I type
    __uint32_t rd;          // for R type

    __uint32_t shmat;       // for R type
    __uint32_t funct;       // for R type

    __uint32_t immed;       // for I type
    
    __uint32_t address;     // for J type

    __uint32_t signExtImm;  // for I type (addi, addiu, lbu, lhu, lw, slti, sltiu, sb, sc, sh, sw, lwcu, ldcl, swcl, sdcl)
    __uint32_t zeroExtImm;  // for I type (andi, ori)

    __uint32_t branchAddr;  // for Branch (beq, bne)
    __uint32_t jumpAddr;    // for Jump (j, jal)
}OPERATOR;

// ref: https://en.wikibooks.org/wiki/MIPS_Assembly/Register_File
typedef struct _REGISTERS {
    __uint32_t r0;          // always zero ($0)
    __uint32_t at;          // reserved for assembler
    
    __uint32_t v0;          // 1st return value ($2)
    __uint32_t v1;          // 2nd return value ($3)

    __uint32_t a0;          // function 1st arg ($4)
    __uint32_t a1;          // function 2nd arg ($5)
    __uint32_t a2;          // function 3rd arg ($6)
    __uint32_t a3;          // function 4th arg ($7)

    __uint32_t t0;          // temporary register ($8) 
    __uint32_t t1;          // temporary register ($9)
    __uint32_t t2;          // temporary register ($10)
    __uint32_t t3;          // temporary register ($11)
    __uint32_t t4;          // temporary register ($12)
    __uint32_t t5;          // temporary register ($13)
    __uint32_t t6;          // temporary register ($14)
    __uint32_t t7;          // temporary register ($15)

    __uint32_t s0;          // saved register ($16)
    __uint32_t s1;          // saved register ($17)
    __uint32_t s2;          // saved register ($18)
    __uint32_t s3;          // saved register ($19)
    __uint32_t s4;          // saved register ($20)
    __uint32_t s5;          // saved register ($21)
    __uint32_t s6;          // saved register ($22)
    __uint32_t s7;          // saved register ($23)

    __uint32_t t8;          // more temporary register ($24)
    __uint32_t t9;          // more temporary register ($25)

    __uint32_t k0;          // reserved for kernel ($26)          
    __uint32_t k1;          // reserved for kernel ($27)

    __uint32_t gp;          // global pointer ($28)
    __uint32_t sp;          // stack pointer ($29)
    __uint32_t fp;          // frame pointer ($30)
    __uint32_t ra;          // return address ($31)
}REGISTERS;