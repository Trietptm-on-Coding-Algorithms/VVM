#include <stdio.h>
#include <stdlib.h>

#define OP_END 0
#define OP_INC_DP 1
#define OP_DEC_DP 2
#define OP_INC_VAL 3
#define OP_DEC_VAL 4
#define OP_OUT 5
#define OP_IN 6
#define OP_JMP_FWD 7
#define OP_JMP_BCK 8

#define PROGRAM_SIZE 1048576
#define MAX_SIZE 5024291
#define DATA_SIZE 1638375

#define STACK_PUSH(A) (stack[sp++] = A)
#define STACK_POP() (stack[--sp])
#define STACK_EMPTY() (sp == 0)
#define STACK_FULL() (sp == MAX_SIZE)

static inline void die(int code, const char* message){
    fprintf(stderr, "%s", message);
    exit(code);
}

struct instruction_t{
    unsigned short operator;
    unsigned short operand;
};

static struct instruction_t program[PROGRAM_SIZE];
static unsigned short stack[MAX_SIZE];
static unsigned int sp = 0;

void compile(FILE* fp){
    unsigned short pc = 0, jmp_pc;
    int ip;
    while ((ip = getc(fp)) != EOF && pc < MAX_SIZE){
        switch (ip) {
            case '>': program[pc].operator = OP_INC_DP; break;
            case '<': program[pc].operator = OP_DEC_DP; break;
            case '+': program[pc].operator = OP_INC_VAL; break;
            case '-': program[pc].operator = OP_DEC_VAL; break;
            case '.': program[pc].operator = OP_OUT; break;
            case ',': program[pc].operator = OP_IN; break;
            case '[':
                program[pc].operator = OP_JMP_FWD;
                if (STACK_FULL())
                    die(1, "Cannot jump forwards: Stack is full.\n");
                STACK_PUSH(pc);
                break;
            case ']':
                if (STACK_EMPTY())
                    die(1, "Cannot jump backwards: Stack is full.\n");
                jmp_pc = STACK_POP();
                program[pc].operator = OP_JMP_BCK;
                program[pc].operand = jmp_pc;
                program[jmp_pc].operand = pc;
                break;
            default:  
                pc--;
                break;
        }
        pc++;
    }
    if(!STACK_EMPTY())
        die(1, "Program ends with non-empty stack.");
    if(pc == MAX_SIZE)
        die(1, "Program exceeds maximum program size.");
    program[pc].operator = OP_END;
}

void execute(){
    unsigned short data[DATA_SIZE], pc = 0;
    unsigned int ptr = DATA_SIZE;
    while (--ptr) data[ptr] = 0;
    while (program[pc].operator != OP_END && ptr < DATA_SIZE){
        switch (program[pc].operator){
            case OP_INC_DP: ptr++; break;
            case OP_DEC_DP: ptr--; break;
            case OP_INC_VAL: data[ptr]++; break;
            case OP_DEC_VAL: data[ptr]--; break;
            case OP_OUT: putchar(data[ptr]); break;
            case OP_IN: data[ptr] = (short unsigned int)getchar(); break;
            case OP_JMP_FWD: if(!data[ptr]) { pc = program[pc].operand; } break;
            case OP_JMP_BCK: if(data[ptr]) { pc = program[pc].operand; } break;
            default: die(2, "Unknown instruction.");
        }
        pc++;
    }
    if(ptr == DATA_SIZE)
        die(1, "Program used up too much memory.");
}

int main(int argc, const char * argv[]){
    FILE *fp;
    if(argc != 2)
        die(1, "Wrong number of arguments supplied.\n");
    if(!(fp = fopen(argv[1], "r")))
        die(1, "File could not be opened.\n");
    compile(fp);
    fclose(fp);
    execute();
    return 0;
}
