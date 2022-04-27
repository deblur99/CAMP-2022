#include "mem_access.h"

u_int32_t* writeIntoMemory(u_int32_t *MEMORY, SCYCLE_HANDLER *handler) {
    switch (handler->inst->opcode) {
        case SW:
            MEMORY[handler->inst->rs + handler->inst->signExtImm] =
                handler->regMemory[handler->inst->rt];
            break;
    }

    return MEMORY;    
}