#include "vm.h"

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


int main(){
    int code[] = { ICONST, 72, PRINT, ICONST, 101, PRINT, ICONST, 108, 
        PRINT, ICONST, 108, PRINT, ICONST, 111, PRINT, ICONST, 44, PRINT, 
        ICONST, 32, PRINT, ICONST, 119, PRINT, ICONST, 111, PRINT, 
        ICONST, 114, PRINT, ICONST, 108, PRINT, ICONST, 100, PRINT, 
        ICONST, 33, GSTORE, 0, GLOAD, 0, PRINT, ICONST, 10, PRINT, HALT };
    vm_execute(factorial, 22, 0, sizeof(factorial) / sizeof(factorial[0]));
    printf("\n");
    vm_execute(code, 0, 1, sizeof(code) / sizeof(code[0]));
    return 0;
}
