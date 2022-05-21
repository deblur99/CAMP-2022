#include "writeback.h"

SCYCLE_HANDLER* updatePC(SCYCLE_HANDLER *handler) {
    switch (handler->inst->optype[0]) {
        case 'R':
            // PC = R[rs]
            if (handler->inst->funct == JR) {
                handler->PC->prevPC = handler->PC->currPC;
                handler->PC->currPC = handler->regMemory[handler->inst->rs];  
                break;
            }
            
    }
}