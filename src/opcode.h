#ifndef OPCODE_H
#define OPCODE_H

enum {
/*Addition operation*/
    ADD = 1,
/*Subtraction operation*/
    SUB,
/*Multiplication operation*/
    MULT,
/*Division operation*/
    DIV,
/*Modulo operation*/
    MOD,
/*< operation*/
    LT,
/*== operation*/
    EQ,
/*> operation*/
    GT,
/*branch operation*/
    BR,
/*branch if true operation*/
    BRT,
/*branch if false operation*/
    BRF,
/*put operation*/
    CONST,
/*load variable operation*/
    LOAD,
/*load global variable operation*/
    GLOAD,
/*store variable operation*/
    STORE,
/*store global variable operation*/
    GSTORE,
/*print operation*/
    PRINT,
/*pop operation*/
    POP,
/*end/halt operation*/
    HALT,
/*<= operation*/
    LEQ,
/*>= operation*/
    GEQ,
/*call subroutine operation*/
    CALL,
/*return from subroutine operation*/
    RET,
/*print integer operation*/
    IPRINT,
/*fetch operation*/
    FETCH,
/*++ operation*/
    INC,
/*-- operation*/
    DEC
};

/**
 * @brief An instruction struct
 *
 * Defines an instruction based on
 * its number of operands and its name.
 */
typedef struct{
    int operands;
    const char* name;
}instruction;

/*sets up the instruction struct*/
instruction* setup_instructions();

#endif
