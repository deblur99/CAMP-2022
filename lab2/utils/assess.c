#include "assess.h"

SCYCLE_HANDLER* updateCounter(SCYCLE_HANDLER *handler) {
    handler->counter->returnValue =
        handler->regMemory[v0];

    handler->counter->executedInst++;    

    switch (handler->inst->optype[0]) {
    case 'R':
        handler->counter->executedRTypeInst++;
        break;
        
    case 'I':
        handler->counter->executedITypeInst++;

        if (handler->inst->opcode == LW ||
            handler->inst->opcode == SW) {
            
            handler->counter->memoryAccessInst++;
        }

        if (handler->inst->opcode == BEQ ||
            handler->inst->opcode == BNE) {
            
            handler->counter->takenBranches++;
        }
        
        break;

    case 'J':
        handler->counter->executedJTypeInst++;
        break;
    }

    return handler;   
}
