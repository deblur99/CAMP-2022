#include "show.h"

int isEmptyInst(INSTRUCT *inst) {
    return (strncmp(inst->optype, "R", 2) != 0 && 
        strncmp(inst->optype, "I", 2) != 0 &&
        strncmp(inst->optype, "J", 2) != 0);
}

void showInstructorAfterDecode(INSTRUCT *inst) {
    printf("=========================\n");
    printf("optype: %s, opcode: 0x%X\n", inst->optype, inst->opcode);

    if (inst->optype[0] == 'R' || inst->optype[0] == 'I') {
        printf("rs: 0x%X, rt: 0x%X\n", inst->rs, inst->rt);
    }

    switch (inst->optype[0])
    {
    case 'R':
        printf("rd: 0x%X, shamt: 0x%X, funct: 0x%X\n",
                inst->rd, inst->shmat, inst->funct);
        break;
        
    case 'I':
        printf("immediate: 0x%X\n", inst->immed);
        break;

    case 'J':
        printf("address: 0x%X\n", inst->address);

    default:
        break;
    }
    printf("=========================\n");
}

void showStatusAfterExecInst(SCYCLE_HANDLER *handler) {
    // TO DO : print status of registers by the below notes
    /*
    A. For each instruction execution,
        i. Changed architectural state
    */

   // 명령어 실행으로 인해 변경된 레지스터의 값을 출력
    printf("%d Cycle, at 0x%08X PC Address\n", handler->counter->cycle, handler->PC->prevPC);
    printf("=========================\n");
    printf("After execution\n");
        
    switch (handler->inst->optype[0]) {
    case 'R':
        printf("(rs) $%d: 0x%08X, ", handler->inst->rs, handler->regMemory[handler->inst->rs]);
        printf("(rt) $%d: 0x%08X, ", handler->inst->rt, handler->regMemory[handler->inst->rt]);
        printf("(rd) $%d: 0x%08X\n", handler->inst->rd, handler->regMemory[handler->inst->rd]);
        break;

    case 'I':
        printf("(rs) $%d: 0x%08X, ", handler->inst->rs, handler->regMemory[handler->inst->rs]);
        printf("(rt) $%d: 0x%08X\n", handler->inst->rt, handler->regMemory[handler->inst->rt]);
        break;

    case 'J':
        break;
    }

    printf("Updated PC: 0x%08X\n", handler->PC->currPC);
    printf("=========================\n\n");
   
    return;
}

void showCounterAfterExecProgram(COUNTER *counter) {
    // TO DO : print status of registers by the below notes
    /*
    B. After the completion of the program
        i. Final return value (value in r2) – up to here, basic requirement
        ii. Number of executed instructions
        iii. Number of (executed) R-type instruction
        iv. Number of I-type instruction
        v. Number of J-type instruction
        vi. Number of memory access instructions -> LW, SW
        vii. Number of taken branches -> BNE, BEQ
    */

    printf("\n******************************************************\n");

    printf("All instructions in the program have been executed.\n");
    printf("======================================================\n");
    printf("1) Final return value ($2) : 0x%08X\n", counter->returnValue);
    printf("2) Number of executed instructions (Total cycles) : %d\n", counter->returnValue);
    printf("3) Number of executed R-type instruction : %d\n", counter->executedRTypeInst);
    printf("4) Number of executed I-type instruction : %d\n", counter->executedITypeInst);
    printf("5) Number of executed J-type instruction : %d\n", counter->executedJTypeInst);
    printf("6) Number of memory access instructions : %d\n", counter->memoryAccessInst);
    printf("7) Number of taken branches : %d\n", counter->takenBranches);
    printf("======================================================\n");

    printf("******************************************************\n");
}