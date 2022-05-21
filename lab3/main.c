/*
4/16 한 일
- 파서 제작
 - 바이너리 파일에서 명령어 한 줄씩 읽어와서, 각 명령어를 리틀 엔디안 구조로 변환한 후 정적 메모리에 저장함
 - 가용 메모리가 충분치 않아 가급적 구동은 Assam 서버에서 하는 걸 추천
 - 현재는 메모리 크기를 0x10000로 잡았지만, 실제 제출용 코드에서는 0xFFFFFFFF로 잡아야 함

이후 해야할 일 (제출마감 4/24 일요일. Freeday는 5일 있음)
1) Decode
2) Execute
3) Write memory
4) Write register
5) Update PC
6) 매 명령어 실행 끝날 때마다 레지스터의 현재 state, 사용한 명령어의 개수 등등 출력
*/

#include "./utils/defines.h"
#include "./utils/init.h"
#include "./utils/show.h"
#include "./stages/fetch.h"
#include "./stages/decode.h"
#include "./stages/execute.h"

u_int32_t *MEMORY;
SCYCLE_HANDLER *handler;

int main(int argc, char *argv[]) {
    // memory allocation
    MEMORY = initMainMemory(); // bring all binary codes from .o file
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
    while (handler->PC->currPC <= 0x40) { // MEMORY_SIZE is set temporarily 0x40
        handler->PC->prevPC = handler->PC->currPC;
        handler->inst = initInstruction();

        handler->inst = decode(handler->inst, fetch(handler->PC->currPC, MEMORY));

        // check inst is nop. if inst is nop, then update PC and pass this instruction.
        if (isEmptyInst(handler->inst)) {
            handler->PC->currPC += 4; // to be writeback function
            freeInstruction(handler->inst);
            continue;
        }

        showInstructorAfterDecode(handler->inst);

        handler = execute(handler);

        handler->PC->currPC += 4; // to be writeback function
        //showStatusAfterExecInst(handler);

        freeInstruction(handler->inst);
    }

    showCounterAfterExecProgram(handler->counter); // print after all executions

    // free allocated memory
    freeHandler(handler);

    return 0;
}