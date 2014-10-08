#include "vm.h"

#define MAX_SIZE 4096
#define TRUE 1
#define FALSE 0
#define DEBUG FALSE

/**
 * @brief disassemble
 * @param sp -> stack pointer
 * @param fp -> function pointer
 * @param ip -> instruction pointer
 * @param opcode -> current opcode
 * @param ins -> instruction struct
 * @param code -> opcodes
 * @param stack -> stack
 *
 * prints current operations and a stack trace.
 * Is invoked if DEBUG is defined.
 */
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

void vm_execute(int code[], int ip, int datasize, unsigned long length){
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
                case IINC:
                    stack[sp]++;
                    break;
                case IDEC:
                    stack[sp]--;
                    break;
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
                case ILEQ:
                    b = stack[sp--];
                    a = stack[sp--];
                    stack[++sp] = a <= b ? TRUE : FALSE;
                    break;
                case IGEQ:
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
                    die(127, "Exit on program failure.");
            }
        }
        return;
}

program vm_parse(char *filename){
    FILE* file = fopen(filename, "r");
    char* line = NULL;
    char** command;
    unsigned long size = 100;
    int* code = malloc(size * sizeof(int));
    unsigned long codep = 0;
    register int i;
    instruction* ins = setup_instructions();
    unsigned int len = IDEC;
    size_t linelength = 0;
    int entry = 0;
    
    if(!file)
        die(127, "Could not open file.");

    while(getline(&line, &linelength, file) != -1){
        short found = FALSE;
        strtok(line, "\n");
        command = str_split(line, ' ');

        if(codep == size){
            size += 100;
            code = (int *) realloc(code, size * sizeof(int));
            if(code == NULL)
                die(127, "Program too big, could not allocate enough storage.");
        }

        if(strcmp(command[0], "ENTRY") == 0){
            entry = (int) strtol(command[1], (char **) NULL, 10);
        }else{
            for(i = 1; i < len; i++)
                if(strcmp(ins[i].name, command[0]) == 0){
                    code[codep++] = i;
                    found = TRUE;
                    break;
                }

            if(found && ins[i].operands > 0){
                int nargs = ins[i].operands;
                for(i = 1; i <= nargs; i++){
                    code[codep++] = (int) strtol(command[i], (char **) NULL, 10);
                }
            }
        }
    }
    
    fclose(file);

    if(line)
        free(line);

    code = (int *) realloc(code, codep * sizeof(int));
    
    program prog = {codep, entry, code};

    return prog;
}

char** str_split(char* a_str, const char a_delim){
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp){
        if (a_delim == *tmp){
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (a_str + strlen(a_str) - 1);

    count++;

    result = malloc(sizeof(char*) * count);

    if (result){
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token){
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
