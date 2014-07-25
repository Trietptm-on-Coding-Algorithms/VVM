#include "vm.h"

#define MAX_SIZE 4096
#define TRUE 1
#define FALSE 0
#define DEBUG FALSE

static inline void disassemble(int sp, int fp, int ip, int opcode, instruction* ins, int code[], int stack[]){
    if(opcode > 0 && opcode < 24)
        printf("%04d: %s(%d)\n", ip, ins[opcode].name, opcode);
    if(ins[opcode].operands == 1)
        printf("\t%d\n", code[ip+1]);
    else if(ins[opcode].operands == 2)
        printf("\t%d, %d\n", code[ip+1], code[ip+2]);
    int i;
    printf("\n===Stack trace===\n");
    for(i = sp; i >= 0; i--)
        printf("%04d: %d\n", i, stack[i]);
    printf("\n");
}

void vm_execute(int code[], int ip, int datasize, int length){
        int data[datasize];
        int stack[MAX_SIZE];
        int sp = -1;
        int fp = -1;
        int nargs, addr, a, b;
        instruction* ins = setup_instructions();
        while(ip < length){
            int opcode = code[ip];
            ip++;
            if(DEBUG){
                if(ip+2 != length)
                    disassemble(sp, fp, ip, opcode, ins, code, stack);
            }
            switch(opcode){
                case LOAD:
                    stack[++sp] = stack[code[ip++]+fp];
                    break;
                case STORE:
                    stack[fp+code[ip++]] = stack[sp--];
                case BR:
                    ip = code[ip];
                    break;
                case BRT:
                    addr = code[ip++];
                    if(stack[sp--] == TRUE) ip = addr;
                    break;
                case BRF:
                    addr = code[ip++];
                    if(stack[sp--] == FALSE) ip = addr;
                    break;
                case IADD:
                    b = stack[sp--];
                    a = stack[sp--];
                    stack[++sp] = a + b;
                    break;
                case ISUB:
                    b = stack[sp--];
                    a = stack[sp--];
                    stack[++sp] = a - b;
                    break;
                case IMULT:
                    b = stack[sp--];
                    a = stack[sp--];
                    stack[++sp] = a * b;
                    break;
                case IDIV:
                    b = stack[sp--];
                    a = stack[sp--];
                    stack[++sp] = a / b;
                    break;
                case IMOD:
                    b = stack[sp--];
                    a = stack[sp--];
                    stack[++sp] = a % b;
                    break;
                case ILT:
                    b = stack[sp--];
                    a = stack[sp--];
                    stack[++sp] = a < b ? TRUE : FALSE;
                    break;
                case IEQ:
                    b = stack[sp--];
                    a = stack[sp--];
                    stack[++sp] = a == b;
                    break;
                case IGT:
                    b = stack[sp--];
                    a = stack[sp--];
                    stack[++sp] = a > b ? TRUE : FALSE;
                    break;
                case LEQ:
                    b = stack[sp--];
                    a = stack[sp--];
                    stack[++sp] = a <= b ? TRUE : FALSE;
                    break;
                case GEQ:
                    b = stack[sp--];
                    a = stack[sp--];
                    stack[++sp] = a >= b ? TRUE : FALSE;
                    break;
                case ICONST:
                    stack[++sp] = code[ip++];
                    break;
                case GLOAD:
                    stack[++sp] = data[code[ip++]];
                    break;
                case GSTORE:
                    data[code[ip++]] = stack[sp--];
                    break;
                case IPRINT:
                    printf("%d", stack[sp--]);
                    break;
                case PRINT:
                    putchar(stack[sp--]);
                    break;
                case FETCH:
                    stack[++sp] = getchar();
                    break;
                case HALT:
                    return;
                case CALL:
                    addr = code[ip++];
                    nargs = code[ip++];
                    stack[++sp] = nargs;
                    stack[++sp] = fp;
                    stack[++sp] = ip;
                    fp = sp;
                    ip = addr;
                    break;
                case RET:
                    addr = stack[sp--];
                    sp = fp;
                    ip = stack[sp--];
                    fp = stack[sp--];
                    nargs = stack[sp--];
                    sp -= nargs;
                    stack[++sp] = addr;
                    break;
                case POP:
                    sp--;
                default:
                    fprintf(stderr, "Unrecognized: %d\n", opcode);
                    die(1, "Exit on program failure.");
            }
        }
        return;
}
