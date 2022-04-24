#include "execute.h"

void execute(INSTRUCT *inst, u_int32_t regMemory[], u_int32_t *PC) {
    switch (inst->optype[0]) {
    case 'R':
        return executeRType(inst, regMemory, PC);
    
    case 'I':
        return executeIType(inst, regMemory, PC);

    case 'J':
        return executeJType(inst, regMemory, PC);

    default:
        return;
    }   
}

void executeRType(INSTRUCT *inst, u_int32_t regMemory[], u_int32_t *PC) {
    if (inst->opcode != RTYPE &&
        inst->opcode != MFC0) {
            return;
        }

    if (inst->opcode == RTYPE) {
        switch (inst->funct) {
        case ADD:
        // R[rd] = R[rs] + R[rt]
            *(regMemory + inst->rd) = *(regMemory + inst->rs) + *(regMemory + inst->rt); 
            return;

        case JR:
        // PC = R[rs]
            *PC = *(regMemory + inst->rs);  
            return;

        default:
            return;
        }
    }
}

void executeIType(INSTRUCT *inst, u_int32_t regMemory[], u_int32_t *PC) {
    switch (inst->opcode) {
    case ADDI:
    // rs가 가리키는 레지스터에 저장되어 있는 값 가져온다.
    // ex) rs가 0x11를 가리키면 0 + 0x11번째, 즉 17번째 레지스터에 저장된 값을 가져온다.
        *(regMemory + inst->rt) = *(regMemory + inst->rs) + (u_int32_t)inst->immed;    // R[rt] = R[rs] + SignExtImm
        return;

    case ADDIU:
        *(regMemory + inst->rt) = *(regMemory + inst->rs) + (u_int32_t)inst->immed;    // R[rt] = R[rs] + SignExtImm
        return;
    
    default:
        return;
    }
}

void executeJType(INSTRUCT *inst, u_int32_t regMemory[], u_int32_t *PC) {
    switch (inst->opcode) {
    case J:
        *PC = inst->address;        // PC = JumpAddr
        return;
    
    case JAL:
        regMemory[ra] = *PC + 0x8;  // R[31] = PC + 8
        *PC = inst->address;        // PC = JumpAddr
        return;

    default:
        return;   
    }
}