#include "decode.h"

INSTRUCT* decode(INSTRUCT *inst, __uint32_t target) {
    // get opcode
    inst->opcode = target >> 26;
    
    if (inst->opcode == RTYPE || inst->opcode == MFC0) {
        return decodeRType(inst, target);
    }
    
    if (inst->opcode == J || inst->opcode == JAL) {
        return decodeJType(inst, target);
    }

    return decodeIType(inst, target);
};

INSTRUCT* decodeRType(INSTRUCT *inst, __uint32_t target) {
    strcpy(inst->optype, "R");
    return inst;
}

INSTRUCT* decodeIType(INSTRUCT *inst, __uint32_t target) {
    strcpy(inst->optype, "I");
    return inst;
}

INSTRUCT* decodeJType(INSTRUCT *inst, __uint32_t target) {
    strcpy(inst->optype, "J");

    inst->address = target << 6 >> 6;

    return inst;
}