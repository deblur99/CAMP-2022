#include "execute.h"

SCYCLE_HANDLER* execute(SCYCLE_HANDLER *handler, u_int32_t *MEMORY) {
    switch (handler->inst->optype[0]) {
    case 'R':
        handler = executeRType(handler);
        break;
    
    case 'I':
        handler = executeIType(handler, MEMORY);
        break;

    case 'J':
        handler = executeJType(handler);
        break;
    }

    return handler;
}

SCYCLE_HANDLER* executeRType(SCYCLE_HANDLER *handler) {
    if (handler->inst->opcode == RTYPE) {
        switch (handler->inst->funct) {
        case MOVE:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rs];
            break;

        // R[rd] = R[rs] + R[rt]
        case ADD:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rs] 
                    + handler->regMemory[handler->inst->rt]; 
            break;

        // R[rd] = R[rs] - R[rt]
        case SUB:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rs] 
                    - handler->regMemory[handler->inst->rt]; 
            break;

        case SUBU:
            handler->regMemory[handler->inst->rd] =  
                handler->regMemory[handler->inst->rs] 
                    - (u_int32_t)(handler->regMemory[handler->inst->rt]); 
            break;

        // R[rd] = R[rs] * R[rt]
        case MULT:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rs] 
                    * handler->regMemory[handler->inst->rt]; 
            break;

        case MULTU:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rs] 
                    * (u_int32_t)(handler->regMemory[handler->inst->rt]); 
            break;

        // R[rd] = R[rs] / R[rt]
        case DIV:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rs] 
                    / handler->regMemory[handler->inst->rt]; 
            break;

        case DIVU:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rs] 
                    / (u_int32_t)(handler->regMemory[handler->inst->rt]); 
            break;


        case AND:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rs] 
                    & handler->regMemory[handler->inst->rt];
            break;

        case NOR:
            handler->regMemory[handler->inst->rd] = 
                ~(handler->regMemory[handler->inst->rs] 
                    | handler->regMemory[handler->inst->rt]);
            break;

        case OR:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rs] 
                    | handler->regMemory[handler->inst->rt];
            break;

        // R[rd] = (R[rs] < R[rt]) ? 1 : 0
        case SLT:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rs] 
                    < handler->regMemory[handler->inst->rt];
            break;
        
        // R[rd] = (R[rs] < R[rt]) ? 1 : 0
        case SLTU:
            handler->regMemory[handler->inst->rd] = 
                (u_int32_t)(handler->regMemory[handler->inst->rs]) 
                    < (u_int32_t)(handler->regMemory[handler->inst->rt]);
            break;

        // R[rd] = R[rt] << shamt
        case SLL:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rt] << handler->inst->shmat;
            break;

        // R[rd] = R[rt] >> shamt
        case SRL:
            handler->regMemory[handler->inst->rd] = 
                handler->regMemory[handler->inst->rt] >> handler->inst->shmat;
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

SCYCLE_HANDLER* executeIType(SCYCLE_HANDLER *handler, u_int32_t *MEMORY) {
    switch (handler->inst->opcode) {
    // R[rt] = immed
    case LI:
        handler->regMemory[handler->inst->rt] 
            = handler->inst->immed;
        break;

    case LW:
    // R[rt] = M[R[rs]+SignExtImm]
        handler->regMemory[handler->inst->rt] = 
            MEMORY[handler->regMemory[handler->inst->rs] 
                + handler->inst->signExtImm];
        break;

    // postpone to the next step : access memory
    case SW:
        break;

    case ADDI:
    // R[rt] = R[rs] + SignExtImm
    // rs가 가리키는 레지스터에 저장되어 있는 값 가져온다.
    // ex) rs가 0x11를 가리키면 0 + 0x11번째, 즉 17번째 레지스터에 저장된 값을 가져온다.
        handler->regMemory[handler->inst->rt] = 
            handler->regMemory[handler->inst->rs] 
                + handler->inst->signExtImm;    
        break;

    case ADDIU:
    // R[rt] = R[rs] + SignExtImm
        handler->regMemory[handler->inst->rt] = 
            handler->regMemory[handler->inst->rs] 
                + (u_int32_t)(handler->inst->signExtImm);
        break;

    case ANDI:
        handler->regMemory[handler->inst->rt] = 
            handler->regMemory[handler->inst->rs] 
                & handler->inst->zeroExtImm;
        break;

    case ORI:
        handler->regMemory[handler->inst->rt] = 
            handler->regMemory[handler->inst->rs]
                | handler->inst->zeroExtImm;
        break;
    
    // PC = PC + 4 + BranchAddr
    // BranchAddr = signExtImm << 2
    case BEQ:
        if (handler->regMemory[handler->inst->rs]
                == handler->regMemory[handler->inst->rt]) {

            handler->PC->prevPC = handler->PC->currPC;

            handler->PC->currPC = handler->PC->currPC 
                + (handler->inst->signExtImm << 2);
        }
        break;

    case BNE:
        if (handler->regMemory[handler->inst->rs]
                != handler->regMemory[handler->inst->rt]) {
            
            handler->PC->prevPC = handler->PC->currPC;

            handler->PC->currPC = handler->PC->currPC 
                + (handler->inst->signExtImm << 2);
        }
        break;
    }

    return handler;
}

SCYCLE_HANDLER* executeJType(SCYCLE_HANDLER *handler) {
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