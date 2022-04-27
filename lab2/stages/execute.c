#include "execute.h"

SCYCLE_HANDLER* execute(SCYCLE_HANDLER *handler) {
    handler->counter->executedInst++;

    switch (handler->inst->optype[0]) {
    case 'R':
        handler = executeRType(handler);
        break;
    
    case 'I':
        handler = executeIType(handler);
        break;

    case 'J':
        handler = executeJType(handler);
        break;
    }

    handler->counter->returnValue = handler->regMemory[v0]; // update v0
    handler->counter->cycle++;                              // cycle number increment

    return handler;
}

SCYCLE_HANDLER* executeRType(SCYCLE_HANDLER *handler) {
    handler->counter->executedRTypeInst++;

    if (handler->inst->opcode == RTYPE) {
        switch (handler->inst->funct) {
        case ADD:
        // R[rd] = R[rs] + R[rt]
            handler->regMemory[handler->inst->rd] = handler->regMemory[handler->inst->rs] + handler->regMemory[handler->inst->rt]; 
            break;

        case JR:
        // PC = R[rs]
            handler->PC->prevPC = handler->PC->currPC;
            handler->PC->currPC = handler->regMemory[handler->inst->rs];  
            break;
        }
    }

    return handler;
}

SCYCLE_HANDLER* executeIType(SCYCLE_HANDLER *handler) {
    handler->counter->executedITypeInst++;

    switch (handler->inst->opcode) {
    case ADDI:
    // rs가 가리키는 레지스터에 저장되어 있는 값 가져온다.
    // ex) rs가 0x11를 가리키면 0 + 0x11번째, 즉 17번째 레지스터에 저장된 값을 가져온다.
        handler->regMemory[handler->inst->rt] = handler->regMemory[handler->inst->rs] + (int32_t)(handler->inst->immed);    // R[rt] = R[rs] + SignExtImm
        break;

    case ADDIU:
        handler->regMemory[handler->inst->rt] = handler->regMemory[handler->inst->rs] + (u_int32_t)(handler->inst->immed);    // R[rt] = R[rs] + SignExtImm
        break;
    }
    
    return handler;
}

SCYCLE_HANDLER* executeJType(SCYCLE_HANDLER *handler) {
    handler->counter->executedJTypeInst++;

    switch (handler->inst->opcode) {
    case J:
        handler->PC->prevPC = handler->PC->currPC;
        handler->PC->currPC = handler->inst->address;        // PC = JumpAddr
        break;
    
    case JAL:
        handler->regMemory[ra] = handler->PC->currPC + 0x8;  // R[31] = PC + 8

        handler->PC->prevPC = handler->PC->currPC;
        handler->PC->currPC = handler->inst->address;        // PC = JumpAddr
        break;
    }

    return handler;
}