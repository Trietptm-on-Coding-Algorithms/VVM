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
int nargs[] = { 0, /* ADD    */
                0, /* SUB    */
                0, /* MULT   */
                0, /* DIV    */
                0, /* MOD    */
                0, /* LT     */
                0, /* EQ     */
                0, /* GT     */
                1, /* BR     */
                1, /* BRT    */
                1, /* BRF    */
                1, /* CONST  */
                1, /* LOAD   */
                1, /* GLOAD  */
                1, /* STORE  */
                1, /* GSTORE */
                0, /* PRINT  */
                0, /* POP    */
                0, /* HALT   */
                0, /* LEQ    */
                0, /* GEQ    */
                2, /* CALL   */
                0, /* RET    */
                0, /* IPRINT */
                0, /* FETCH  */
                0, /* INC    */
                0  /* DEC    */
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
