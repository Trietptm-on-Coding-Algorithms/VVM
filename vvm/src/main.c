#include "vm.h"

/*A factorial program in my dsl*/
int factorial[] = {
    LOAD, -3,
    ICONST, 2,
    ILT,
    BRF, 10,
    ICONST, 1,
    RET,
    LOAD, -3,
    LOAD, -3,
    ICONST, 1,
    ISUB,
    CALL, 0, 1,
    IMULT,
    RET,
    ICONST, 12,
    CALL, 0, 1,
    IPRINT,
    HALT
};

/**
 * @brief main
 * argc -> the argument counter
 * argv -> the argument vector
 *
 * executes code in my dsl.
 */
int main(int argc, char** argv){
    /*int code[] = { ICONST, 72, PRINT, ICONST, 101, PRINT, ICONST, 108, 
        PRINT, ICONST, 108, PRINT, ICONST, 111, PRINT, ICONST, 44, PRINT, 
        ICONST, 32, PRINT, ICONST, 119, PRINT, ICONST, 111, PRINT, 
        ICONST, 114, PRINT, ICONST, 108, PRINT, ICONST, 100, PRINT, 
        ICONST, 33, GSTORE, 0, GLOAD, 0, PRINT, ICONST, 10, PRINT, HALT };
    vm_execute(factorial, 22, 0, sizeof(factorial) / sizeof(factorial[0]));
    printf("\n");
    vm_execute(code, 0, 1, sizeof(code) / sizeof(code[0]));*/
    if(argc != 2)
        die(127, "Please specify exactly one file to execute.");
    program prog = vm_parse(argv[1]);
    vm_execute(prog.code, 22, 0, prog.length);
    return 0;
}
