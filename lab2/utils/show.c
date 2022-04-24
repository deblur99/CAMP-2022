#include "show.h"

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

void showCurrentStatus(REGISTERS *regs) {
    // TO DO : print status of registers by the below notes
    /*
    A. For each instruction execution,
        i. Changed architectural state
    B. After the completion of the program
        i. Final return value (value in r2) – up to here, basic requirement
        ii. Number of executed instructions
        iii. Number of (executed) R-type instruction
        iv. Number of I-type instruction
        v. Number of J-type instruction
        vi. Number of memory access instructions
        vii. Number of taken branches
    */
    return;
}