#include <stdlib.h>

#include "opcode.h"

const char* opcodes[] = {"IADD", "ISUB", "IMULT", "IDIV", "IMOD",
    "ILT", "IEQ", "IGT", "BR", "BRT", "BRF", "ICONST", "LOAD", "GLOAD",
    "STORE", "GSTORE", "PRINT", "POP", "HALT", "LEQ", "GEQ", "CALL", "RET",
    "IPRINT", "FETCH", "IINC", "IDEC" };

int nargs[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 
    0, 2, 0, 0, 0, 0, 0 };

instruction* setup_instructions(){
    int i;
    static instruction ins[IDEC+1];
    ins[0].operands = 0;
    ins[0].name = NULL;
    for(i = 1; i <= IDEC; i++){
        ins[i].operands = nargs[i-1];
        ins[i].name = opcodes[i-1];
    }
    return ins;
}
