#include <stdio.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <sys/types.h>

#define MEMORY_SIZE         0xFFFFFFFF // original size is 0xFFFFFFFF
#define INST_MEMORY_SIZE    0x10000000
#define REG_MEMORY_SIZE     0x20       // values in each $0~$31 registers

#define OPCODE_MASK     0x000000FF

// opcode list (from MIPS Green Sheet)
// nop
#define EMPTY           0x00000000

// R type OPCODE is 0 (except MFC0. its opcode is 0x10)
#define RTYPE           0x0

// R type funct
#define MOVE            0x21

#define ADD             0x20 
#define ADDU            0x21
#define AND             0x24
#define JR              0x8
#define NOR             0x27
#define OR              0x25
#define SLT             0x2A
#define SLTU            0x2B
#define SLL             0x0
#define SRL             0x2
#define SUB             0x22
#define SUBU            0x23
#define DIV             0x1A
#define DIVU            0x1B
#define MFHI            0x10
#define MFLO            0x12
#define MFC0            0x10    // (except) OPCODE is 0x10. funct is 0x0
#define MULT            0x18
#define MULTU           0x19
#define SRA             0x3

// I type opcode
#define ADDI            0x8
#define ADDIU           0x9
#define ANDI            0xC
#define BEQ             0x4
#define BNE             0x5
#define LBU             0x24
#define LHU             0x25
#define LL              0x30
#define LUI             0xF
#define LW              0x23
#define ORI             0xD
#define SLTI            0xA
#define SLTIU           0xB
#define SB              0x28
#define SC              0x38
#define SH              0x29
#define SW              0x2B
#define LWCL            0x31
#define LDCL            0x35
#define SWCL            0x39
#define SDCL            0x3D

// J type opcode
#define J               0x2
#define JAL             0x3

using namespace std;

// actual size is 0xFFFFFFFF
int32_t *MEMORY;
u_int32_t *INST_MEMORY;

int32_t REG_MEMORY[REG_MEMORY_SIZE] = {0, };

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
    u_int32_t opcode = 0x0;         // for R, I, J type

    u_int32_t rs = 0x0;             // for R, I type          
    u_int32_t rt = 0x0;             // for R, I type
    u_int32_t rd = 0x0;             // for R type

    u_int32_t shmat = 0x0;          // for R type
    u_int32_t funct = 0x0;          // for R type

    int16_t immed = 0x0;            // for I type

    int32_t signExtImm = 0x0;       // for I type (addi, addiu, lbu, lhu, lw, slti, sltiu, sb, sc, sh, sw, lwcu, ldcl, swcl, sdcl)
    int32_t zeroExtImm = 0x0;       // for I type (andi, ori)

    bool isMetBranchCond = false;   // for Branch (beq, bne)
    u_int32_t branchAddr = 0x0;     // for Branch (beq, bne)

    u_int32_t jumpAddr = 0x0;       // for Jump (j, jal)
};

class Counter {
protected:
    int32_t returnValue = 0;

    u_int32_t executedInst = 0;
    u_int32_t executedRTypeInst = 0;
    u_int32_t executedITypeInst = 0;
    u_int32_t executedJTypeInst = 0;
    u_int32_t memoryAccessInst = 0;

    u_int32_t takenBranches = 0;
};

enum LatchSpecifier {
    IF_ID = 0,
    ID_EXE,
    EX_MEM,
    MEM_WB
};

class LatchFieldData {
private:
    bool IN_VALID;
    bool OUT_VALID;

    // All latches
    u_int32_t   _NPC =          0x0;
    
    // IF/ID
    u_int32_t   _IR =           0x0;

    // ID/EXE, EX/MEM, MEM/WB
    u_int32_t   _rd =           0x0;

    // ID/EXE, EX/MEM
    int32_t     _ReadData2 =    0x0;

    // ID/EXE
    int32_t     _ReadData1 =    0x0;
    u_int32_t   _opcode =       0x0;
    u_int32_t   _rs =           0x0;
    u_int32_t   _rt =           0x0;
    u_int32_t   _shamt =        0x0;
    u_int32_t   _funct =        0x0;
    int32_t     _immed =        0x0;
    
    // EX/MEM, MEM/WB
    int32_t     _ALU_Result =   0x0;

    // MEM/WB
    int32_t     _ReadData   =   0x0; // for LW inst.

public:
    // IF/ID
    LatchFieldData(
        int type,
        u_int32_t NPC,
        u_int32_t IR
    ) {
        if (type == IF_ID) {
            _NPC = _NPC;
            _IR = IR;
            IN_VALID = true;
            OUT_VALID = true;
        }
    }

    // ID/EXE
    LatchFieldData(
        int type,
        int32_t ReadData1,
        int32_t ReadData2,
        u_int32_t opcode,
        u_int32_t rs,
        u_int32_t rt,
        u_int32_t rd,
        u_int32_t shamt,
        u_int32_t funct,
        int32_t immed,
    ) {
        if (type == ID_EXE) {
            _NPC = NPC;
            _ReadData1 = ReadData1;
            _ReadData2 = ReadData2;
            _opcode = opcode;
            _rt = rt;
            _rs = rs;
            _rd = rd;
            _shamt = shamt;
            _funct = funct;
            _immed = immed;
        }

        IN_VALID = true;
        OUT_VALID = true;
    }

    // EX/MEM, MEM/WB
    LatchFieldData(
        int type,
        u_int32_t NPC,
        int32_t ALU_Result,
        int32_t ReadData,
        u_int32_t rd
    ) {
        // EX/MEM
        if (type == EX_MEM) {
            _NPC = NPC;
            _ALU_Result = ALU_Result;
            _ReadData2 = ReadData;
            _rd = rd;
        }

        // MEM/WB
        if (type == MEM_WB) {
            _NPC = NPC;
            _ALU_Result = ALU_Result;
            _ReadData = ReadData;
            _rd = rd;
        }

        IN_VALID = true;
        OUT_VALID = true;
    }

    getLatchFieldData()

    LatchFieldData getIF_ID_LatchData() {
        return 
    }
}

Latch *l = new Latch(1).makeLatch();

class Latch {
private:
    int _type;
    LatchFieldData *lfData;
protected:
    virtual void update() = 0;
    virtual void clear() = 0;
    virtual void showStatus() = 0;
public:
    Latch(int type) {
        _type = type;
        return Latch()
    }

    Latch* makeLatch() {
        switch (type)
        {
        case IF_ID:
            return new IF_ID_Latch();
        
        case ID_EXE:
            return new IF_EXE_Latch();
        
        case EX_MEM:
            return new EX_MEM_Latch();
        
        case MEM_WB:
            return new MEM_WB_Latch();

        default:
            return nullptr;
        }
    }
};

class IF_ID_Latch: public Latch {
private:
    u_int32_t _NPC = 0x0;
    u_int32_t _IR = 0x0;

public:
    IF_ID_Latch(u_int32_t NPC, u_int32_t IR) {
        update(NPC, IR);
    }

    void update(u_int32_t NPC, u_int32_t IR) {
        _NPC = NPC;
        _IR = IR;
    }

    void clear() {
        _NPC = 0x0;
        _IR = 0x0;
    }

    void showStatus() {
        printf("======================================================\n");
        printf("IF/ID Latch Status\n");
        printf("* Next Program Counter:\t0x%08X\n", _NPC);
        printf("* Current Instruction:\t0x%08X\n", _IR);
        printf("======================================================\n");
    }
};

class ID_EX_Latch: public Latch {
private:
    u_int32_t _NPC = 0x0;
    u_int32_t _ReadData1 = 0x0;
    u_int32_t _ReadData2 = 0x0;
    u_int32_t _opcode = 0x0;
    u_int32_t _rs = 0x0;
    u_int32_t _rt = 0x0;
    u_int32_t _rd = 0x0;
    u_int32_t _shamt = 0x0;
    u_int32_t _funct = 0x0;
    u_int32_t _immed = 0x0;

public:
    ID_EX_Latch(u_int32_t NPC, u_int32_t IR) {
        update(NPC, IR);
    }

    void update(u_int32_t NPC, u_int32_t IR) {
        _NPC = NPC;
        _IR = IR;
    }

    void clear() {
        _NPC = 0x0;
        _IR = 0x0;
    }

    void showStatus() {
        printf("======================================================\n");
        printf("IF/ID Latch Status\n");
        printf("======================================================\n");
    }
};

class EX_MEM_Latch: public Latch {
private:
    u_int32_t _NPC = 0x0;
    u_int32_t _IR = 0x0;

public:
    EX_MEM_Latch(u_int32_t NPC, u_int32_t IR) {
        update(NPC, IR);
    }

    void update(u_int32_t NPC, u_int32_t IR) {
        _NPC = NPC;
        _IR = IR;
    }

    void clear() {
        _NPC = 0x0;
        _IR = 0x0;
    }

    void showStatus() {
        printf("======================================================\n");
        printf("IF/ID Latch Status\n");
        printf("======================================================\n");
    }
};

class MEM_WB_Latch: public Latch {
private:
    u_int32_t _NPC = 0x0;
    u_int32_t _IR = 0x0;

public:
    MEM_WB_Latch(u_int32_t NPC, u_int32_t IR) {
        update(NPC, IR);
    }

    void update(u_int32_t NPC, u_int32_t IR) {
        _NPC = NPC;
        _IR = IR;
    }

    void clear() {
        _NPC = 0x0;
        _IR = 0x0;
    }

    void showStatus() {
        printf("======================================================\n");
        printf("IF/ID Latch Status\n");
        printf("* Next Program Counter:\t0x%08X\n", _NPC);
        printf("* Current Instruction:\t0x%08X\n", _IR);
        printf("======================================================\n");
    }
};

class Simulator: public Inst, Counter {
private:
    FILE *fp;
    string _filename;
    int amount = 0;

    u_int32_t PC = 0x0 - 4;
    u_int32_t inst = 0x0;

    int32_t writeValIntoMemory = 0x0; // for writeback stage

    void initMEMORY() {
        MEMORY = new int32_t[(u_int64_t)MEMORY_SIZE];
        for (int i = 0; i < (u_int64_t)MEMORY_SIZE; i++) {
            MEMORY[i] = 0xFFFFFFFF;
        }
    }

    void initINST_MEMORY() {
        INST_MEMORY = new u_int32_t[(u_int64_t)INST_MEMORY_SIZE];
        for (int i = 0; i < (u_int64_t)INST_MEMORY_SIZE; i++) {
            INST_MEMORY[i] = 0xFFFFFFFF;
        }
    }

    void initREG_MEMORY() {
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
    
        fclose(fp); 
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
        if (inst == EMPTY) {
            return;
        }

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
            case MOVE:
                writeValIntoMemory = REG_MEMORY[rs];
                break;

            // R[rd] = R[rs] + R[rt]
            case ADD:
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
                (int32_t)REG_MEMORY[rs] < signExtImm;
            break;
        
        case SLTIU:
            writeValIntoMemory = 
                REG_MEMORY[rs] < (u_int32_t)signExtImm;
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
        initMEMORY();
        initINST_MEMORY();
        initREG_MEMORY();

        _filename = filename;
        loadFile();
    }

    ~Simulator() {
        delete MEMORY;
        delete INST_MEMORY;
    }

    void run(bool debug) {
        if (fp == NULL) return;

        while (PC != 0xFFFFFFFF) {
            fetch();
            if (debug) showPcAfterUpdating();
            if (debug) showInstructorAfterFetch();

            decode();
            if (debug) showInstructorAfterDecode();

            execute();
            if (debug) showStatusAfterExecInst();

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

    // Laptop
    // Simulator s("/mnt/c/Users/deblu/CAMP/lab3/test_prog/input4.bin");

    // Home
    // Simulator s("/mnt/c/Users/32184893/CAMP-2022/lab3/test_prog/simple2.bin");

    // Assam
    Simulator s("/home/hyeonmin18/CAMP-2022/lab3/test_prog/input4.bin");

    s.run(doDebug);

    return 0;
}
