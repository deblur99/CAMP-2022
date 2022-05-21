/******************
 * 22일 토요일 한 일
 * C 코드를 C++ 코드로 옮기기로 하였음 -> 코드 관리의 어려움
 * JR 명령어 관련 문제점 발견 : 0xFFFFFFFF를 저장하고 +4를 해서 0x03이 PC에 저장됨
 * 
 * 23일 일요일 할 일
 * execute, access memory, writeback 재구성하기
 * single cycle 제대로 돌아가는지 확인하기
 * 될 때까지 고치기
 * 다 되면 in, out latch 추가하기
 * pipelining 구현하기
*******************/

#include "defines.h"

using namespace std;

int32_t MEMORY[0x10000000]; // actual size is 0xFFFFFFFF
u_int32_t REG_MEMORY[0x20];

enum Register {
    zero = 0x0,                   // always zero ($0)
    at = 0x1,                     // reserved for assembler ($1)
    
    // 1st, 2nd return value registers ($2, $3)
    v0 = 0x2, v1,                 

    // argument registers for function ($4 ~ $7)
    a0 = 0x4, a1, a2, a3,         

    // temporary registers ($8 ~ $15)
    t0 = 0x8, t1, t2, t3, t4, t5, t6, t7,

    // saved temporary registers ($16 ~ $23)
    s0 = 0x10, s1, s2, s3, s4, s5, s6, s7,

    // temporary registers ($24, $25)
    t8 = 0x18, t9,          

    // reserved for kernel ($26, $27)
    k0 = 0x1A, k1,

    gp = 0x1C,                      // global pointer ($28). 0x1C
    sp = 0x1D,                      // stack pointer ($29). 0x1D
    fp = 0x1E,                      // frame pointer ($30). 0x1E
    ra = 0x1F,                      // return address ($31). 0x1F
};

class Inst {
protected:
    char optype;
    u_int32_t opcode;      // for R, I, J type

    u_int32_t rs;          // for R, I type          
    u_int32_t rt;          // for R, I type
    u_int32_t rd;          // for R type

    u_int32_t shmat;       // for R type
    u_int32_t funct;       // for R type

    int16_t immed;         // for I type
    
    u_int32_t address;     // for J type

    int32_t signExtImm;    // for I type (addi, addiu, lbu, lhu, lw, slti, sltiu, sb, sc, sh, sw, lwcu, ldcl, swcl, sdcl)
    int32_t zeroExtImm;    // for I type (andi, ori)

    u_int32_t branchAddr;  // for Branch (beq, bne)
    u_int32_t jumpAddr;    // for Jump (j, jal)
};

class Counter {
protected:
    int32_t returnValue;

    u_int32_t executedInst;
    u_int32_t executedRTypeInst;
    u_int32_t executedITypeInst;
    u_int32_t executedJTypeInst;
    u_int32_t memoryAccessInst;

    u_int32_t takenBranches;
};

class Simulator: public Inst, Counter {
private:
    FILE *fp;
    string _filename;
    int amount = 0;

    u_int32_t PC = 0x0;
    u_int32_t inst;

    void initRegMemory() {
        REG_MEMORY[sp] = 0x1000000;
        REG_MEMORY[ra] = 0xFFFFFFFF;
    }

    void loadFile() {
        vector<char> directory(_filename.begin(), _filename.end());
        directory.push_back('\0');
        char *d_ptr = &directory[0];

        if ((fp = fopen(d_ptr, "rb")) == NULL) {
            perror("File Not Found");
            return;
        }
        
        while (fread(&MEMORY[4 * amount++], 1, sizeof(int), fp) == 4) {
            ;
        }

        printf("=========================\n");
        printf("All loaded data from .bin\n");
        printf("=========================\n");
        // print all loaded data from MEMORY array
        for (int i = 0; i < amount; i++) {
            printf("0x%X: 0x%08X\n", 4 * i, MEMORY[4 * i]);
        }
        printf("=========================\n");
    
        fclose(fp);
    }

    void fetch() {   
        u_int32_t inst = MEMORY[PC];

        u_int32_t result =          0x00000000;

        u_int32_t r1     = inst & 0xFF000000;
        u_int32_t r2     = inst & 0x00FF0000;
        u_int32_t r3     = inst & 0x0000FF00;
        u_int32_t r4     = inst & 0x000000FF;

        result = result | (r1 >> 24);
        result = result | (r2 >> 8);
        result = result | (r3 << 8);
        result = result | (r4 << 24);

        inst = result;

        printf("0x%08X\n", inst); // test
    }   

    void decode() {
        if (inst == EMPTY) {
            return;
        }

        // opcode
        opcode = inst >> 26;
        inst = inst << 6 >> 6; // strip opcode

        if (opcode == RTYPE || opcode == MFC0) {
            return decodeRType();
        }

        if (opcode == J || opcode == JAL) {
            return decodeJType();
        }

        return decodeIType();
    }

    void decodeRType() {
        // optype
        optype = 'R';

        // rs
        rs = inst >> 21;
        inst = inst << 11 >> 11; // strip rs

        // rt
        rt = inst >> 16;
        inst = inst << 16 >> 16; // strip rt

        // rd
        rd = inst >> 11;
        inst = inst << 21 >> 21; // strip rd

        // shamt
        shmat = inst >> 6;
        inst = inst << 26 >> 26; // strip shamt

        // funct
        funct = inst;
    }

    void decodeIType() {
        optype = 'I';

        // rs
        rs = inst >> 21;
        inst = inst << 11 >> 11; // strip rs

        // rt
        rt = inst >> 16;
        inst = inst << 16 >> 16; // strip rt

        // immediate
        immed = (int16_t)(inst);

        // signExtImm
        u_int32_t MSB = immed >> 15 << 16;
        u_int32_t result = 0x00000000;

        result = result | inst;
        for (int i = 0; i < 16; i++) {
            result  = result | MSB;
            MSB = MSB << 1;
        }
        signExtImm = result;

        result = 0x0; // init

        // ZeroExtImm
        result = (u_int32_t)(immed) | 0x00000000;
        zeroExtImm = result;
    }

    void decodeJType() {
        optype = 'J';

        // address
        address = inst;
    }

    void execute() {
        switch (optype) {
        case 'R':
            return executeRType();

        case 'I':
            return executeIType();

        case 'J':
            return executeJType();
        }
    }

    void executeRType() {
        if (opcode == RTYPE) {
            switch (funct) {
            case MOVE:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rs];
                break;

            // R[rd] = R[rs] + R[rt]
            case ADD:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rs] 
                        + handler->regMemory[handler->inst->rt]; 
                break;

            // R[rd] = R[rs] - R[rt]
            case SUB:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rs] 
                        - handler->regMemory[handler->inst->rt]; 
                break;

            case SUBU:
                handler->regMemory[handler->inst->rd] =  
                    handler->regMemory[handler->inst->rs] 
                        - (u_int32_t)(handler->regMemory[handler->inst->rt]); 
                break;

            // R[rd] = R[rs] * R[rt]
            case MULT:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rs] 
                        * handler->regMemory[handler->inst->rt]; 
                break;

            case MULTU:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rs] 
                        * (u_int32_t)(handler->regMemory[handler->inst->rt]); 
                break;

            // R[rd] = R[rs] / R[rt]
            case DIV:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rs] 
                        / handler->regMemory[handler->inst->rt]; 
                break;

            case DIVU:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rs] 
                        / (u_int32_t)(handler->regMemory[handler->inst->rt]); 
                break;


            case AND:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rs] 
                        & handler->regMemory[handler->inst->rt];
                break;

            case NOR:
                handler->regMemory[handler->inst->rd] = 
                    ~(handler->regMemory[handler->inst->rs] 
                        | handler->regMemory[handler->inst->rt]);
                break;

            case OR:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rs] 
                        | handler->regMemory[handler->inst->rt];
                break;

            // R[rd] = (R[rs] < R[rt]) ? 1 : 0
            case SLT:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rs] 
                        < handler->regMemory[handler->inst->rt];
                break;

            // R[rd] = (R[rs] < R[rt]) ? 1 : 0
            case SLTU:
                handler->regMemory[handler->inst->rd] = 
                    (u_int32_t)(handler->regMemory[handler->inst->rs]) 
                        < (u_int32_t)(handler->regMemory[handler->inst->rt]);
                break;

            // R[rd] = R[rt] << shamt
            case SLL:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rt] << handler->inst->shmat;
                break;

            // R[rd] = R[rt] >> shamt
            case SRL:
                handler->regMemory[handler->inst->rd] = 
                    handler->regMemory[handler->inst->rt] >> handler->inst->shmat;
                break;

            case JR:
            // PC = R[rs]
                break;               
            }
        }

        return;
    }

    void executeIType(SCYCLE_HANDLER *handler, MAIN_MEMORY *mainMemory) {
        switch (handler->inst->opcode) {
        case LW:
        // R[rt] = M[R[rs]+SignExtImm]
            handler->regMemory[handler->inst->rt] = 
                mainMemory->MEMORY[handler->regMemory[handler->inst->rs] 
                    + handler->inst->signExtImm];
            break;

        // postpone to the next step : access memory
        case SW:
            break;

        case ADDI:
        // R[rt] = R[rs] + SignExtImm
        // rs가 가리키는 레지스터에 저장되어 있는 값 가져온다.
        // ex) rs가 0x11를 가리키면 0 + 0x11번째, 즉 17번째 레지스터에 저장된 값을 가져온다.
            handler->regMemory[handler->inst->rt] = 
                handler->regMemory[handler->inst->rs] 
                    + handler->inst->signExtImm;    
            break;

        case ADDIU:
        // R[rt] = R[rs] + SignExtImm
            handler->regMemory[handler->inst->rt] = 
                handler->regMemory[handler->inst->rs] 
                    + (u_int32_t)(handler->inst->signExtImm);
            break;

        case ANDI:
            handler->regMemory[handler->inst->rt] = 
                handler->regMemory[handler->inst->rs] 
                    & handler->inst->zeroExtImm;
            break;

        case ORI:
            handler->regMemory[handler->inst->rt] = 
                handler->regMemory[handler->inst->rs]
                    | handler->inst->zeroExtImm;
            break;

        // PC = PC + 4 + BranchAddr
        // BranchAddr = signExtImm << 2
        case BEQ:
            if (handler->regMemory[handler->inst->rs]
                    == handler->regMemory[handler->inst->rt]) {

                handler->PC->prevPC = handler->PC->currPC;

                handler->PC->currPC = handler->PC->currPC 
                    + (handler->inst->signExtImm << 2);
            }
            break;

        case BNE:
            if (handler->regMemory[handler->inst->rs]
                    != handler->regMemory[handler->inst->rt]) {
                    
                handler->PC->prevPC = handler->PC->currPC;

                handler->PC->currPC = handler->PC->currPC 
                    + (handler->inst->signExtImm << 2);
            }
            break;
        }

        return;
    }

    void executeJType(SCYCLE_HANDLER *handler) {
        switch (handler->inst->opcode) {
        case J:
            handler->PC->prevPC = handler->PC->currPC;
            handler->PC->currPC = handler->inst->address;        // PC = JumpAddr
            break;

        case JAL:
            handler->regMemory[ra] = handler->PC->currPC + 0x8;  // R[31] = PC + 8

            handler->PC->prevPC = handler->PC->currPC;
            handler->PC->currPC = handler->inst->address;        // PC = JumpAddr
            break;
        }

        return;
    }

    void accessMemory() {

    }

    void writeback() {
        PC += 4;
    }

public:
    Simulator(string filename) {
        _filename = filename;

        loadFile();
    }

    void run() {
        initRegMemory();

        // debug
        for (int i = 0; i < 10; i++) {
            fetch();
            decode();
            execute();
            accessMemory();
            writeback();
        }
    }
};

int main() {
    Simulator s("/mnt/c/Users/deblu/CAMP/new_lab3/test_prog/simple.bin");
    s.run();

    return 0;
}