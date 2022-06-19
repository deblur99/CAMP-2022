#include "./utils/defines.h"
#include "./utils/inst.h"
#include "./utils/counter.h"

using namespace std;

int32_t MEMORY[MEMORY_SIZE];
u_int32_t INST_MEMORY[INST_MEMORY_SIZE];

int32_t REG_MEMORY[REG_MEMORY_SIZE];

class Simulator: public Inst, Counter {
private:
    FILE *fp;
    string _filename;
    int amount = 0;

    u_int32_t PC = 0x0;
    u_int32_t inst = 0x0;

    int32_t writeValIntoMemory = 0x0; // for writeback stage

    void initMemory() {
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
        
        int size = 0;
        while (fread(&INST_MEMORY[4 * size++], 1, sizeof(int), fp) == 4) {
            ;
        }

        printf("=========================\n");
        printf("All loaded data from .bin\n");
        printf("=========================\n");
        // print all loaded data from MEMORY array
        for (int i = 0; i < size; i++) {
            printf("0x%X: 0x%08X\n", 4 * i, INST_MEMORY[4 * i]);
        }
        printf("=========================\n");
    }

    void closeFile() {
        if (fp != NULL) fclose(fp);
    }

    void updatePC() {
        if (optype == 'R' && funct == JR) {
            PC = REG_MEMORY[rs];
            return;
        }

        if (opcode == J || opcode == JAL) {
            PC = 4 * jumpAddr;
            return;
        }

        // if "Condition" meets in BNE, BEQ inst
        if (isMetBranchCond) {
            PC = (PC + 4) + (signExtImm << 2);
            isMetBranchCond = false;
            return;
        }

        // the rest case, generally PC gets plus 4
        PC += 4;
    }

    void fetch() {
        inst = INST_MEMORY[PC];

        u_int32_t result =        0x00000000;

        u_int32_t r1     = inst & 0xFF000000;
        u_int32_t r2     = inst & 0x00FF0000;
        u_int32_t r3     = inst & 0x0000FF00;
        u_int32_t r4     = inst & 0x000000FF;

        result = result | (r1 >> 24);
        result = result | (r2 >> 8);
        result = result | (r3 << 8);
        result = result | (r4 << 24);

        inst = result;
    }   

    void decode() {
        // opcode
        opcode = inst >> 26;
        inst = inst & 0x03FFFFFF; // strip opcode

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
        inst = inst & 0x1FFFFF; // strip rs

        // rt
        rt = inst >> 16;
        inst = inst & 0xFFFF; // strip rt

        // rd
        rd = inst >> 11;
        inst = inst & 0x7FF; // strip rd

        // shamt
        shmat = inst >> 6;
        inst = inst & 0x3F; // strip shamt

        // funct
        funct = inst;
    }

    void decodeIType() {
        optype = 'I';

        // rs
        rs = inst >> 21;
        inst = inst & 0x1FFFFF; // strip rs

        // rt
        rt = inst >> 16;
        inst = inst & 0xFFFF; // strip rt

        // immediate
        immed = inst;

        // signExtImm
        int32_t MSB = immed >> 15 << 15;
        int32_t result = 0x00000000;

        result = result | inst;
        for (int i = 0; i < 16; i++) {
            result  = result | MSB;
            MSB = MSB << 1;
        }
        signExtImm = result;

        result = 0x0; // init

        // ZeroExtImm;
        result = (int32_t)immed & 0x0000FFFF;
        zeroExtImm = result;
    }

    void decodeJType() {
        optype = 'J';
        jumpAddr = inst;
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
            // R[rd] = R[rs] + R[rt]
            case ADD:
                writeValIntoMemory =
                    REG_MEMORY[rs] + REG_MEMORY[rt];     
                break;

            case ADDU:
                writeValIntoMemory =
                    REG_MEMORY[rs] + REG_MEMORY[rt];    
                break;

            // R[rd] = R[rs] - R[rt]
            case SUB:
                writeValIntoMemory = 
                    REG_MEMORY[rs] - REG_MEMORY[rt]; 
                break;

            case SUBU:
                writeValIntoMemory =  
                    (u_int32_t)(REG_MEMORY[rs] - REG_MEMORY[rt]); 
                break;

            // R[rd] = R[rs] * R[rt]
            case MULT:
                writeValIntoMemory = 
                    REG_MEMORY[rs] * REG_MEMORY[rt]; 
                break;

            case MULTU:
                writeValIntoMemory = 
                    REG_MEMORY[rs] * (u_int32_t)(REG_MEMORY[rt]); 
                break;

            // R[rd] = R[rs] / R[rt]
            case DIV:
                writeValIntoMemory = 
                    REG_MEMORY[rs] / REG_MEMORY[rt]; 
                break;

            case DIVU:
                writeValIntoMemory = 
                    REG_MEMORY[rs] / (u_int32_t)(REG_MEMORY[rt]); 
                break;

            case AND:
                writeValIntoMemory = 
                    REG_MEMORY[rs] & REG_MEMORY[rt];
                break;

            case NOR:
                writeValIntoMemory = 
                    ~(REG_MEMORY[rs] | REG_MEMORY[rt]);
                break;

            case OR:
                writeValIntoMemory = 
                    REG_MEMORY[rs] | REG_MEMORY[rt];
                break;

            // R[rd] = (R[rs] < R[rt]) ? 1 : 0
            case SLT:
                writeValIntoMemory = 
                    REG_MEMORY[rs] < REG_MEMORY[rt];
                break;

            // R[rd] = (R[rs] < R[rt]) ? 1 : 0
            case SLTU:
                writeValIntoMemory = 
                    (u_int32_t)(REG_MEMORY[rs]) < (u_int32_t)(REG_MEMORY[rt]);
                break;

            // R[rd] = R[rt] << shamt
            case SLL:
                writeValIntoMemory = REG_MEMORY[rt] << shmat;
                break;

            // R[rd] = R[rt] >> shamt
            case SRL:
                writeValIntoMemory = REG_MEMORY[rt] >> shmat;
                break;

            case JR:
            // PC = R[rs]
                break;               
            }
        }

        return;
    }

    void executeIType() {
        // postpone to the next step : access memory
        switch (opcode) {
        case LW:
            break;

        case SW:
            break;

        case LUI:
            writeValIntoMemory =
                (int32_t)(immed) << 16;
            break;

        case ADDI:
        // R[rt] = R[rs] + SignExtImm
        // rs가 가리키는 레지스터에 저장되어 있는 값 가져온다.
        // ex) rs가 0x11를 가리키면 0 + 0x11번째, 즉 17번째 레지스터에 저장된 값을 가져온다.
            writeValIntoMemory = 
                REG_MEMORY[rs] + immed;    
            break;

        case ADDIU:
        // R[rt] = R[rs] + SignExtImm
            writeValIntoMemory = 
                REG_MEMORY[rs] + immed;
            break;

        case ANDI:
            writeValIntoMemory = 
                REG_MEMORY[rs] & zeroExtImm;
            break;

        case ORI:
            writeValIntoMemory = 
                REG_MEMORY[rs] | zeroExtImm;
            break;

        case SLTI:
            writeValIntoMemory = 
                REG_MEMORY[rs] < signExtImm;
            break;
        
        case SLTIU:
            writeValIntoMemory = 
                (u_int32_t)REG_MEMORY[rs] < (u_int32_t)signExtImm;
            break;

        // PC = PC + 4 + BranchAddr
        // BranchAddr = signExtImm << 2
        case BEQ:
            isMetBranchCond = (REG_MEMORY[rs] == REG_MEMORY[rt]);
            break;

        case BNE:
            isMetBranchCond = (REG_MEMORY[rs] != REG_MEMORY[rt]);
                                //|| (REG_MEMORY[rs] != 0x0);
            break;
        }

        return;
    }

    // JAL의 경우 execute 단계에서 ra 레지스터에 현재 PC값 + 8 저장하고,
    // writeback 단계에서 JumpAddr 저장한다.
    void executeJType() {
        switch (opcode) {
        case J:
            break;

        case JAL:
            writeValIntoMemory = PC + 0x8;  // R[31] = PC + 8
            break;
        }

        return;
    }

    void accessMemory() {
        if (opcode == LW || opcode == LWCL)
            return readFromMemory();

        if (opcode == SW)
            return writeIntoMemory();
    }

    void readFromMemory() {
        writeValIntoMemory = MEMORY[REG_MEMORY[rs] + signExtImm];
    }

    void writeIntoMemory() {
        MEMORY[REG_MEMORY[rs] + signExtImm] = REG_MEMORY[rt];
    }

    void writeback() {
        switch (optype) {
        case 'R':
            return writebackRType();

        case 'I':
            return writebackIType();

        case 'J':
            return writebackJType();
        }
    }

    void writebackRType() {
        if (inst == NOP) {
            return;
        }

        if (opcode == RTYPE && funct != JR) {
            REG_MEMORY[rd] = writeValIntoMemory;
        }
    }

    void writebackIType() {
        if (opcode != SW && opcode != BEQ && opcode != BNE)
            REG_MEMORY[rt] = writeValIntoMemory;
    }

    // JAL의 경우 execute 단계에서 ra 레지스터에 현재 PC값 + 8 저장하고,
    // writeback 단계에서 JumpAddr 저장한다.
    void writebackJType() {
        if (opcode == JAL)
            REG_MEMORY[ra] = writeValIntoMemory;
    }

    void updateCounter() {
        returnValue = REG_MEMORY[v0];

        executedInst++;    

        switch (optype) {
        case 'R':
            if (inst != 0x00000000)
                executedRTypeInst++;
            break;

        case 'I':
            executedITypeInst++;

            if (opcode == LW || opcode == SW)
                memoryAccessInst++;

            if (opcode == BEQ || opcode == BNE)
                takenBranches++;

            break;

        case 'J':
            executedJTypeInst++;
            break;
        }   
    }

    void showPcAfterUpdating() {
        printf("Updated PC: 0x%08X\n", PC);
    }

    void showInstructorAfterFetch() {
        printf("[Fetch] %d Cycle, at 0x%08X PC Address\n", executedInst + 1, PC);
    }

    void showInstructorAfterDecode() {
        printf("[Decode] ");
        printf("optype: %c, opcode: 0x%X", optype, opcode);

        if (optype == 'R' || optype == 'I') {
            printf(", rs: 0x%X (%d), rt: 0x%X (%d)", rs, rs, rt, rt);
        }

        switch (optype)
        {
        case 'R':
            printf(", rd: 0x%X (%d), shamt: 0x%X, funct: 0x%X\n",
                    rd, rd, shmat, funct);
            break;

        case 'I':
            printf(", immediate: 0x%X (%d)\n", immed, immed);
            break;

        case 'J':
            if (opcode == J || opcode == JR || opcode == JAL)
                printf(", address: 0x%X\n", jumpAddr);
                break;

        default:
            printf("\n");
            break;
        }
    }

    void showStatusAfterExecInst() {      
        printf("[Execute] ");

        switch (optype) {
        case 'R':
            printf("(rs) $%d: 0x%08X (%d), ", rs, REG_MEMORY[rs], REG_MEMORY[rs]);
            printf("(rt) $%d: 0x%08X (%d), ", rt, REG_MEMORY[rt], REG_MEMORY[rt]);
            printf("(rd) $%d: 0x%08X (%d)", rd, REG_MEMORY[rd], REG_MEMORY[rd]);
            break;

        case 'I':
            printf("(rs) $%d: 0x%08X (%d), ", rs, REG_MEMORY[rs], REG_MEMORY[rs]);
            printf("(rt) $%d: 0x%08X (%d)", rt, REG_MEMORY[rt], REG_MEMORY[rt]);
            break;

        case 'J':
            break;
        }

        printf("\n=========================\n");
    
        return;
    }

    // about counting
    void showCounterAfterExecProgram() {
        printf("\n******************************************************\n");

        printf("All instructions in the program have been executed.\n");
        printf("======================================================\n");
        printf("1) Final return value ($2) : %d\n", REG_MEMORY[v0]);
        printf("2) Number of executed instructions (Total cycles) : %d\n", executedInst);
        printf("3) Number of executed R-type instruction : %d\n", executedRTypeInst);
        printf("4) Number of executed I-type instruction : %d\n", executedITypeInst);
        printf("5) Number of executed J-type instruction : %d\n", executedJTypeInst);
        printf("6) Number of memory access instructions : %d\n", memoryAccessInst);
        printf("7) Number of taken branches : %d\n", takenBranches);
        printf("======================================================\n");

        printf("******************************************************\n");
    }

public:
    Simulator(string filename) {
        _filename = filename;
        loadFile();

        initMemory();
    }

    ~Simulator() {
        closeFile();
    }

    void run(bool debug, int startLoggingIdx) {
        if (fp == NULL) return;

        bool approc = false; // debug

        while (PC != 0xFFFFFFFF) {
            // debug
            if (!approc && (PC >= startLoggingIdx)) {
                printf("approached %d\n", startLoggingIdx);
                approc = true;
            }

            fetch();
            
            if (debug && approc) {
                showPcAfterUpdating();
                showInstructorAfterFetch();
            }

            decode();

            if (debug && approc)
                showInstructorAfterDecode();

            execute();

            if (debug && approc)
                showStatusAfterExecInst();

            accessMemory();
            writeback();

            updatePC();
            
            updateCounter();
        }

        showCounterAfterExecProgram();
    }
};

int main() {
    bool doDebug = false;

    Simulator s("/home/hyeonmin18/CAMP-2022/lab4/test_prog/input4.bin");
    s.run(doDebug, 0x0);

    return 0;
}
