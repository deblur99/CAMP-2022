#include "execute.h"

REGISTERS* execute(INSTRUCT *inst, REGISTERS *regs, u_int32_t regMemory[]) {
    switch (inst->optype[0]) {
    case 'R':
        return executeRType(inst, regs, regMemory);
    
    case 'I':
        return executeIType(inst, regs, regMemory);

    case 'J':
        return executeJType(inst, regs);

    default:
        return regs;
    }   
}

REGISTERS* executeRType(INSTRUCT *inst, REGISTERS *regs, u_int32_t regMemory[]) {
    switch (inst->opcode) {
    case ADD:
        
        break;
    
    default:
        break;
    }

    return regs;
}

REGISTERS* executeIType(INSTRUCT *inst, REGISTERS *regs, u_int32_t regMemory[]) {
    switch (inst->opcode) {
    case ADDI:
    // rs가 가리키는 레지스터에 저장되어 있는 값 가져온다.
    // ex) rs가 0x11를 가리키면 0 + 0x11번째, 즉 17번째 레지스터에 저장된 값을 가져온다.
        *(regMemory + inst->rt) = *(regMemory + inst->rs) + inst->immed;
        regs->

    case ADDIU:
        /* code */
        break;
    
    default:
        break;
    }

    return regs;
}

REGISTERS* executeJType(INSTRUCT *inst, REGISTERS *regs) {
    switch (inst->opcode) {
    case J:
        regs->PC = inst->address;  // PC = JumpAddr
        return regs;
    
    case JAL:
        regs->ra = regs->PC + 0x8; // R[31] = PC + 8
        regs->PC = inst->address;  // PC = JumpAddr
        return regs;

    default:
        return regs;   
    }
}