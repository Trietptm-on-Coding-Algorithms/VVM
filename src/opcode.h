#ifndef OPCODE_H
#define OPCODE_H
enum {
/*Addition operation*/
    IADD = 1,
/*Subtraction operation*/
    ISUB,
/*Multiplication operation*/
    IMULT,
/*Division operation*/
    IDIV,
/*Modulo operation*/
    IMOD,
/*< operation*/
    ILT,
/*== operation*/
    IEQ,
/*> operation*/
    IGT,
/*branch operation*/
    BR,
/*branch if true operation*/
    BRT,
/*branch if false operation*/
    BRF,
/*put operation*/
    ICONST,
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
    ILEQ,
/*>= operation*/
    IGEQ,
/*call subroutine operation*/
    CALL,
/*return from subroutine operation*/
    RET,
/*print integer operation*/
    IPRINT,
/*fetch operation*/
    FETCH,
/*++ operation*/
    IINC,
/*-- operation*/
    IDEC
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
