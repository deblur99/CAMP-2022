#include "mem_access.h"

MAIN_MEMORY* writeIntoMemory(MAIN_MEMORY *mainMemory, SCYCLE_HANDLER *handler) {
    switch (handler->inst->opcode) {
        case SW:
            mainMemory->MEMORY[handler->inst->rs + handler->inst->signExtImm] =
                handler->regMemory[handler->inst->rt];
            break;
    }

    return mainMemory;    
}