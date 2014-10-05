#include <stdio.h>
#include <stdlib.h>

#define END 0
#define INC_DP 1
#define DEC_DP 2
#define INC_VAL 3
#define DEC_VAL 4
#define OUT 5
#define IN 6
#define JMP_FWD 7
#define JMP_BCK 8

#define PROGRAM_SIZE 1048576
#define MAX_SIZE 5024291
#define DATA_SIZE 1638375

#define PUSH(A) (stack[sp++] = A)
#define POP() (stack[--sp])
#define EMPTY() (sp == 0)
#define FULL() (sp == MAX_SIZE)

static inline void die(int code, const char* message){
    fprintf(stderr, "%s", message);
    exit(code);
}

struct instruction{
    unsigned int operator;
    unsigned int operand;
};

static struct instruction program[PROGRAM_SIZE];
static unsigned int stack[MAX_SIZE];
static unsigned long sp = 0;

void compile(FILE* fp){
    unsigned int pc = 0, jmp_pc;
    int ip;
    while ((ip = getc(fp)) != EOF && pc < MAX_SIZE){
        switch (ip) {
            case '>': program[pc].operator = INC_DP; break;
            case '<': program[pc].operator = DEC_DP; break;
            case '+': program[pc].operator = INC_VAL; break;
            case '-': program[pc].operator = DEC_VAL; break;
            case '.': program[pc].operator = OUT; break;
            case ',': program[pc].operator = IN; break;
            case '[':
                program[pc].operator = JMP_FWD;
                if (FULL())
                    die(127, "nbfi:compile: Cannot jump forwards: Stack is full.\n");
                PUSH(pc);
                break;
            case ']':
                if (EMPTY())
                    die(127, "nbfi:compile: Cannot jump backwards: Stack is full.\n");
                jmp_pc = POP();
                program[pc].operator = JMP_BCK;
                program[pc].operand = jmp_pc;
                program[jmp_pc].operand = pc;
                break;
            default:  
                if(ip > 32 && ip < 128)
                    fprintf(stderr, "nbfi:compile: Warning: Unknown command: %c\n", ip);
                pc--;
                break;
        }
        pc++;
    }
    if(!EMPTY())
        die(117, "nbfi:compile: Program ends with non-empty stack.");
    if(pc == MAX_SIZE)
        die(127, "nbfi:compile: Program exceeds maximum program size.");
    program[pc].operator = END;
}

void execute(){
    unsigned int data[DATA_SIZE], pc = 0;
    unsigned int ptr = DATA_SIZE;
    while (--ptr) data[ptr] = 0;
    while (program[pc].operator != END && ptr < DATA_SIZE){
        switch (program[pc].operator){
            case INC_DP: ptr++; break;
            case DEC_DP: ptr--; break;
            case INC_VAL: data[ptr]++; break;
            case DEC_VAL: data[ptr]--; break;
            case OUT: putchar((signed int)data[ptr]); break;
            case IN: data[ptr] = (unsigned int)getchar(); break;
            case JMP_FWD: if(!data[ptr]) { pc = program[pc].operand; } break;
            case JMP_BCK: if(data[ptr]) { pc = program[pc].operand; } break;
            default: die(127, "nbfi:execute: Unknown instruction.\n");
        }
        pc++;
    }
    if(ptr == DATA_SIZE)
        die(127, "nbfi:execute: Program used up too much memory.\n");
}

int main(int argc, const char * argv[]){
    FILE *fp;
    if(argc != 2)
        die(127, "nbfi:parse: Wrong number of arguments supplied.\n");
    if(!(fp = fopen(argv[1], "r")))
        die(1, "nbfi:parse: File could not be opened.\n");
    compile(fp);
    fclose(fp);
    execute();
    return 0;
}
