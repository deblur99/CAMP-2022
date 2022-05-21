#include "decode.h"

void decode(INSTRUCT *inst, u_int32_t target) {
    // check target is 0x00000000 (empty)
    if (target == EMPTY) {
        return;
    }

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

void decodeRType(INSTRUCT *inst, u_int32_t target) {
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
    
    return;
}

void decodeIType(INSTRUCT *inst, u_int32_t target) {
    strcpy(inst->optype, "I");
    
    // rs
    inst->rs = target >> 21;
    target = target << 11 >> 11; // strip rs

    // rt
    inst->rt = target >> 16;
    target = target << 16 >> 16; // strip rt

    // immediate
    inst->immed = (int16_t)(target);

    // signExtImm
    u_int32_t MSB = inst->immed >> 15 << 16;
    u_int32_t result = 0x00000000;

    result = result | target;
    for (int i = 0; i < 16; i++) {
        result  = result | MSB;
        MSB = MSB << 1;
    }
    inst->signExtImm = result;

    result = 0x0; // init

    // ZeroExtImm
    result = (u_int32_t)(inst->immed) | 0x00000000;
    inst->zeroExtImm = result;

    return;
}

void decodeJType(INSTRUCT *inst, u_int32_t target) {
    strcpy(inst->optype, "J");

    // address
    inst->address = target;

    return;
}