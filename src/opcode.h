#ifndef OPCODE_H
#define OPCODE_H
/*Addition operation*/
#define IADD 1
/*Subtraction operation*/
#define ISUB 2
/*Multiplication operation*/
#define IMULT 3
/*Division operation*/
#define IDIV 4
/*Modulo operation*/
#define IMOD 5
/*< operation*/
#define ILT 6
/*== operation*/
#define IEQ 7
/*> operation*/
#define IGT 8
/*branch operation*/
#define BR 9
/*branch if true operation*/
#define BRT 10
/*branch if false operation*/
#define BRF 11
/*put operation*/
#define ICONST 12
/*load variable operation*/
#define LOAD 13
/*load global variable operation*/
#define GLOAD 14
/*store variable operation*/
#define STORE 15
/*store global variable operation*/
#define GSTORE 16
/*print operation*/
#define PRINT 17
/*pop operation*/
#define POP 18
/*end/halt operation*/
#define HALT 19
/*<= operation*/
#define ILEQ 20
/*>= operation*/
#define IGEQ 21
/*call subroutine operation*/
#define CALL 22
/*return from subroutine operation*/
#define RET 23
/*print integer operation*/
#define IPRINT 24
/*fetch operation*/
#define FETCH 25 
/*++ operation*/
#define IINC 26
/*-- operation*/
#define IDEC 27 

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
