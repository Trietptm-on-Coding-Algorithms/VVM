#include "vm.h"

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
    int i;
    if(opcode > 0 && opcode < 24)
        printf("%04d: %s(%d)\n", ip, ins[opcode].name, opcode);
    if(ins[opcode].operands == 1)
        printf("\t%d\n", code[ip+1]);
    else if(ins[opcode].operands == 2)
        printf("\t%d, %d\n", code[ip+1], code[ip+2]);
    printf("\n===Stack trace===\n");
    for(i = sp; i >= 0; i--)
        printf("%04d: %d\n", i, stack[i]);
    printf("\n");
}

void vm_execute(int code[], int ip, int datasize, unsigned long length){
        int* data = (int *) alloca((size_t)datasize * sizeof(int));
        int stack[MAX_SIZE];
        register int sp = -1;
        register int fp = -1;
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

program vm_compile(char *filename){
    FILE* file = fopen(filename, "r");
    char* line = NULL;
    char** command = NULL;
    unsigned long size = 100;
    int* code = malloc(size * sizeof(int));
    int* code_realloc = NULL;
    unsigned long codep = 0;
    register int i;
    instruction* ins = setup_instructions();
    unsigned int len = IDEC;
    size_t linelength = 0;
    int entry = 0;
    program prog;
    
    if(!file)
        die(127, "Could not open file.");

    while(getline(&line, &linelength, file) != -1){
        short found = FALSE;
        unsigned int elemc = 0;
        strtok(line, "\n");
        command = str_split(line, ' ', &elemc);

        if(codep == size){
            size += 100;
            code_realloc = (int *) realloc(code, size * sizeof(int));
            if(code_realloc == NULL){
                free(code);
                die(127, "Program too big, could not allocate enough storage.");
            }
            code = code_realloc;
            code_realloc = NULL;
        }

        if(strcmp(command[0], "ENTRY") == 0){
            entry = (int) strtol(command[1], (char **) NULL, 10);
        }else{
            if(strcmp("\n", command[0]) == 0)
                continue;
            for(i = 1; i < len; i++)
                if(strcmp(ins[i].name, command[0]) == 0){
                    code[codep++] = i;
                    found = TRUE;
                    break;
                }

            if(found == FALSE){
                fprintf(stderr, 
                        "Line %s called with unknown command: %s\n", 
                        line, command[0]);
                die(127, "Compilation failed.");
            }

            if(ins[i].operands > 0){
                int nargs = ins[i].operands;
                if(nargs != elemc-1){
                    fprintf(stderr, 
                            "Line %s called with wrong number of arguments (got %d, expected %d)\n", 
                            line, elemc-1, nargs);
                    die(127, "Compilation failed.");
                }
                for(i = 1; i <= nargs; i++){
                    code[codep++] = (int) strtol(command[i], (char **) NULL, 10);
                }
            }
        }
    }
    
    fclose(file);

    if(line)
        free(line);

    code_realloc = (int *) realloc(code, codep * sizeof(int));
    if(code_realloc != NULL){
        code = code_realloc;
        code_realloc = NULL;
    }
    
    prog.length = codep;
    prog.entrypoint = entry;
    prog.code = code;

    if(command != NULL)
        free(command);

    return prog;
}

char** str_split(char* a_str, const char a_delim, unsigned int* elemc){
    char** result    = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp){
        if (a_delim == *tmp){
            (*elemc)++;
            last_comma = tmp;
        }
        tmp++;
    }

    *elemc += last_comma < (a_str + strlen(a_str) - 1);

    (*elemc)++;

    result = malloc(sizeof(char*) * *elemc);

    if (result){
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token){
            assert(idx < *elemc);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == *elemc - 1);
        *(result + idx) = 0;
    }

    (*elemc)--;

    return result;
}
