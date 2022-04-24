#include "decode.h"

INSTRUCT* decode(INSTRUCT *inst, u_int32_t target) {
    // opcode
    inst->opcode = target >> 26;
    target = target << 6 >> 6; // strip opcode
    
    if (inst->opcode == RTYPE || inst->opcode == MFC0) {
        return decodeRType(inst, target);
    }
    
    if (inst->opcode == J || inst->opcode == JAL) {
        return decodeJType(inst, target);
    }

    return decodeIType(inst, target);
};

INSTRUCT* decodeRType(INSTRUCT *inst, u_int32_t target) {
    // optype
    strcpy(inst->optype, "R");

    // rs
    inst->rs = target >> 21;
    target = target << 11 >> 11; // strip rs

    // rt
    inst->rt = target >> 16;
    target = target << 16 >> 16; // strip rt

    // rd
    inst->rd = target >> 11;
    target = target << 21 >> 21; // strip rd

    // shamt
    inst->shmat = target >> 6;
    target = target << 26 >> 26; // strip shamt

    // funct
    inst->funct = target;
    
    return inst;
}

INSTRUCT* decodeIType(INSTRUCT *inst, u_int32_t target) {
    strcpy(inst->optype, "I");
    
    // rs
    inst->rs = target >> 21;
    target = target << 11 >> 11; // strip rs

    // rt
    inst->rt = target >> 16;
    target = target << 16 >> 16; // strip rt

    // immediate
    inst->immed = target;

    return inst;
}

INSTRUCT* decodeJType(INSTRUCT *inst, u_int32_t target) {
    strcpy(inst->optype, "J");

    // address
    inst->address = target;

    return inst;
}