#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <alloca.h>

#include "util.h"
#include "opcode.h"
#include "vm.h"

/**
 * @brief disassemble
 * @param sp     -> stack pointer
 * @param fp     -> function pointer
 * @param ip     -> instruction pointer
 * @param opcode -> current opcode
 * @param ins    -> instruction struct
 * @param code   -> opcodes
 * @param stack  -> stack
 *
 * prints current operations and a stack trace.
 * Is invoked if DEBUG is defined.
 */
static inline void disassemble(int sp, int fp, int ip, instruction* ins, int code[], int stack[]){
    int i;
    int opcode = code[ip];
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

  /*
    this uses dispatch tables as described in
    https://eli.thegreenplace.net/2012/07/12/computed-goto-for-efficient-dispatch-tables/
  */
  static void* dispatch_table[] = {
    NULL, &&do_add, &&do_sub, &&do_mult, &&do_div, &&do_mod, &&do_lt, &&do_eq,
    &&do_gt, &&do_br, &&do_brt, &&do_brf, &&do_const, &&do_load, &&do_gload,
    &&do_store, &&do_gstore, &&do_print, &&do_pop, &&do_halt, &&do_leq,
    &&do_geq, &&do_call, &&do_ret, &&do_iprint, &&do_fetch, &&do_inc, &&do_dec
  };
  #define DISPATCH() goto *dispatch_table[code[ip++]]

  DISPATCH();
  while(ip < length){
    if(DEBUG_ON && ip+2 != length) disassemble(sp, fp, ip, ins, code, stack);
    do_inc:
        stack[sp]++;
        DISPATCH();
    do_dec:
        stack[sp]--;
        DISPATCH();
    do_load:
        stack[++sp] = stack[code[ip++]+fp];
        DISPATCH();
    do_store:
        stack[fp+code[ip++]] = stack[sp--];
    do_br:
        ip = code[ip];
        DISPATCH();
    do_brt:
        addr = code[ip++];
        if(stack[sp--] == TRUE) ip = addr;
        DISPATCH();
    do_brf:
        addr = code[ip++];
        if(stack[sp--] == FALSE) ip = addr;
        DISPATCH();
    do_add:
        b = stack[sp--];
        a = stack[sp--];
        stack[++sp] = a + b;
        DISPATCH();
    do_sub:
        b = stack[sp--];
        a = stack[sp--];
        stack[++sp] = a - b;
        DISPATCH();
    do_mult:
        b = stack[sp--];
        a = stack[sp--];
        stack[++sp] = a * b;
        DISPATCH();
    do_div:
        b = stack[sp--];
        a = stack[sp--];
        stack[++sp] = a / b;
        DISPATCH();
    do_mod:
        b = stack[sp--];
        a = stack[sp--];
        stack[++sp] = a % b;
        DISPATCH();
    do_lt:
        b = stack[sp--];
        a = stack[sp--];
        stack[++sp] = a < b ? TRUE : FALSE;
        DISPATCH();
    do_eq:
        b = stack[sp--];
        a = stack[sp--];
        stack[++sp] = a == b;
        DISPATCH();
    do_gt:
        b = stack[sp--];
        a = stack[sp--];
        stack[++sp] = a > b ? TRUE : FALSE;
        DISPATCH();
    do_leq:
        b = stack[sp--];
        a = stack[sp--];
        stack[++sp] = a <= b ? TRUE : FALSE;
        DISPATCH();
    do_geq:
        b = stack[sp--];
        a = stack[sp--];
        stack[++sp] = a >= b ? TRUE : FALSE;
        DISPATCH();
    do_const:
        stack[++sp] = code[ip++];
        DISPATCH();
    do_gload:
        stack[++sp] = data[code[ip++]];
        DISPATCH();
    do_gstore:
        data[code[ip++]] = stack[sp--];
        DISPATCH();
    do_iprint:
        printf("%d", stack[sp--]);
        DISPATCH();
    do_print:
        putchar(stack[sp--]);
        DISPATCH();
    do_fetch:
        stack[++sp] = getchar();
        DISPATCH();
    do_halt:
        return;
    do_call:
        addr = code[ip++];
        nargs = code[ip++];
        stack[++sp] = nargs;
        stack[++sp] = fp;
        stack[++sp] = ip;
        fp = sp;
        ip = addr;
        DISPATCH();
    do_ret:
        addr = stack[sp--];
        sp = fp;
        ip = stack[sp--];
        fp = stack[sp--];
        nargs = stack[sp--];
        sp -= nargs;
        stack[++sp] = addr;
        DISPATCH();
    do_pop:
        sp--;
        DISPATCH();
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
    unsigned int len = DEC;
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
