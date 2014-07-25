#ifndef OPCODE_H
#define OPCODE_H
#define IADD 1
#define ISUB 2
#define IMULT 3
#define IDIV 4
#define IMOD 5
#define ILT 6
#define IEQ 7
#define IGT 8
#define BR 9
#define BRT 10
#define BRF 11
#define ICONST 12
#define LOAD 13
#define GLOAD 14
#define STORE 15
#define GSTORE 16
#define PRINT 17
#define POP 18
#define HALT 19
#define LEQ 20
#define GEQ 21
#define CALL 22
#define RET 23
#define IPRINT 24
#define FETCH 25 

typedef struct{
    int operands;
    const char* name;
}instruction;

instruction* setup_instructions();

#endif
