#include "mem_access.h"

int accessMemory(u_int32_t opcode) {
    return opcode == LW ||
        opcode == LWCL ||
        opcode == SW;
}

SCYCLE_HANDLER* readFromMemory(MAIN_MEMORY *mainMemory, SCYCLE_HANDLER *handler) {
    if (!accessMemory(handler->inst->opcode)) {
        return handler;
    }

    if (handler->inst->opcode == LW &&
         handler->inst->opcode == LWCL) {
        handler->regMemory[handler->inst->rt] =
            mainMemory->MEMORY[handler->inst->rs 
                + handler->inst->signExtImm];         
    }

    return handler;
}

MAIN_MEMORY* writeIntoMemory(MAIN_MEMORY *mainMemory, SCYCLE_HANDLER *handler) {
    if (!accessMemory(handler->inst->opcode)) {
        return handler;
    }

    switch (handler->inst->opcode) {
        case SW:
            mainMemory->MEMORY[handler->inst->rs + handler->inst->signExtImm] =
                handler->regMemory[handler->inst->rt];
            break;
    }

    return mainMemory;
}

