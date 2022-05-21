#include "show.h"

int isEmptyInst(INSTRUCT *inst) {
    return (strncmp(inst->optype, "R", 2) != 0 && 
        strncmp(inst->optype, "I", 2) != 0 &&
        strncmp(inst->optype, "J", 2) != 0);
}

void showInstructorAfterFetch(SCYCLE_HANDLER *handler) {
    printf("[Fetch] %d Cycle, at 0x%08X PC Address\n", handler->counter->executedInst + 1, handler->PC->prevPC);
}

void showInstructorAfterDecode(INSTRUCT *inst) {
    printf("[Decode] ");
    printf("optype: %s, opcode: 0x%X", inst->optype, inst->opcode);

    if (inst->optype[0] == 'R' || inst->optype[0] == 'I') {
        printf(", rs: 0x%X, rt: 0x%X", inst->rs, inst->rt);
    }

    switch (inst->optype[0])
    {
    case 'R':
        printf(", rd: 0x%X, shamt: 0x%X, funct: 0x%X\n",
                inst->rd, inst->shmat, inst->funct);
        break;
        
    case 'I':
        printf(", immediate: 0x%X\n", inst->immed);
        break;

    case 'J':
        printf(", address: 0x%X\n", inst->address);

    default:
        printf("\n");
        break;
    }
}

void showStatusAfterExecInst(SCYCLE_HANDLER *handler) {      
    printf("[Execute] ");
        
    switch (handler->inst->optype[0]) {
    case 'R':
        printf("(rs) $%d: 0x%08X, ", handler->inst->rs, handler->regMemory[handler->inst->rs]);
        printf("(rt) $%d: 0x%08X, ", handler->inst->rt, handler->regMemory[handler->inst->rt]);
        printf("(rd) $%d: 0x%08X, ", handler->inst->rd, handler->regMemory[handler->inst->rd]);
        break;

    case 'I':
        printf("(rs) $%d: 0x%08X, ", handler->inst->rs, handler->regMemory[handler->inst->rs]);
        printf("(rt) $%d: 0x%08X, ", handler->inst->rt, handler->regMemory[handler->inst->rt]);
        break;

    case 'J':
        break;
    }

    printf("Updated PC: 0x%08X\n", handler->PC->currPC);
    printf("=========================\n");
   
    return;
}

void showCounterAfterExecProgram(SCYCLE_HANDLER *handler) {
    printf("\n******************************************************\n");

    printf("All instructions in the program have been executed.\n");
    printf("======================================================\n");
    printf("1) Final return value ($2) : 0x%X\n", handler->regMemory[v0]);
    printf("2) Number of executed instructions (Total cycles) : %d\n", handler->counter->executedInst);
    printf("3) Number of executed R-type instruction : %d\n", handler->counter->executedRTypeInst);
    printf("4) Number of executed I-type instruction : %d\n", handler->counter->executedITypeInst);
    printf("5) Number of executed J-type instruction : %d\n", handler->counter->executedJTypeInst);
    printf("6) Number of memory access instructions : %d\n", handler->counter->memoryAccessInst);
    printf("7) Number of taken branches : %d\n", handler->counter->takenBranches);
    printf("======================================================\n");

    printf("******************************************************\n");
}