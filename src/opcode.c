#include "opcode.h"

/*The opcodes as chars*/
const char* opcodes[] = { "ADD", 
                          "SUB", 
                          "MULT", 
                          "DIV", 
                          "MOD",
                          "LT", 
                          "EQ", 
                          "GT", 
                          "BR", 
                          "BRT", 
                          "BRF", 
                          "CONST", 
                          "LOAD", 
                          "GLOAD",
                          "STORE", 
                          "GSTORE", 
                          "PRINT", 
                          "POP", 
                          "HALT", 
                          "LEQ", 
                          "GEQ", 
                          "CALL", 
                          "RET",
                          "IPRINT", 
                          "FETCH", 
                          "INC", 
                          "DEC" 
                        };

/*The argument counter for the opcodes*/
int nargs[] = { 0, 
                0, 
                0, 
                0, 
                0, 
                0, 
                0, 
                0, 
                1, 
                1, 
                1, 
                1, 
                1, 
                1, 
                1, 
                1, 
                0, 
                0, 
                0, 
                0, 
                0, 
                2, 
                0, 
                0, 
                0, 
                0, 
                0 
              };

instruction* setup_instructions(){
    int i;
    static instruction ins[DEC+1];
    ins[0].operands = 0;
    ins[0].name = "\0";
    for(i = 1; i <= DEC; i++){
        ins[i].operands = nargs[i-1];
        ins[i].name = opcodes[i-1];
    }
    return ins;
}
