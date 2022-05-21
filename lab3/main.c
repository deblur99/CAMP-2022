#include "./utils/defines.h"
#include "./utils/init.h"
#include "./utils/assess.h"
#include "./utils/show.h"
#include "./stages/fetch.h"
#include "./stages/decode.h"
#include "./stages/execute.h"
#include "./stages/mem_access.h"

MAIN_MEMORY *mainMemory = NULL;
SCYCLE_HANDLER *handler = NULL;

int main(int argc, char *argv[]) {
    // memory allocation
    mainMemory = initMainMemory(); // bring all binary codes from .o file
    handler = initHandler();

    handler->regMemory = initRegMemory();
    handler->regMemory[sp] = 0x1000000;
    handler->regMemory[ra] = 0xFFFFFFFF;

    handler->PC = initPC();
    handler->PC->prevPC = 0x00000000;
    handler->PC->currPC = 0x00000000;

    handler->inst = initInstruction();

    handler->counter = initCounter();

    // Main tasks: all single cycles execution
    // when PC points 0xFFFFFFFF, then terminate the program.
    while (handler->PC->currPC < MEMORY_SIZE &&
            handler->PC->currPC < 4 * mainMemory->endPoint) {

        handler->PC->prevPC = handler->PC->currPC;
        handler->inst = initInstruction();

        showInstructorAfterFetch(handler);

        handler->inst = decode(handler->inst, fetch(handler->PC->currPC, mainMemory));

        showInstructorAfterDecode(handler->inst);

        handler = execute(handler, mainMemory);

        mainMemory = writeIntoMemory(mainMemory, handler);
        
        if (handler->inst->optype[0] != 'J')
            handler->PC->currPC += 4; // to be writeback function
        showStatusAfterExecInst(handler);
        handler = updateCounter(handler);

        freeInstruction(handler->inst);
    }

    showCounterAfterExecProgram(handler); // print after all executions

    // free allocated memory
    freeHandler(handler);
    freeMainMemory(mainMemory);

    return 0;
}