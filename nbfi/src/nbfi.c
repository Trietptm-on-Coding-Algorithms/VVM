#include <stdio.h>
#include <stdlib.h>

/*end instruction*/
#define END 0
/*increment pointer instruction, equivalent to > */
#define INC_DP 1
/*decrement pointer instruction, equivalent to < */
#define DEC_DP 2
/*increment value instruction, equivalent to + */
#define INC_VAL 3
/*decrement value instruction, equivalent to - */
#define DEC_VAL 4
/*output value instruction, equivalent to . */
#define OUT 5
/*input value instruction, equivalent to , */
#define IN 6
/*jump forward instruction, equivalent to [ */
#define JMP_FWD 7
/*jump back instruction, equivalent to ] */
#define JMP_BCK 8

/*Maximum program size*/
#define PROGRAM_SIZE 1048576
/*Maximum stack size*/
#define MAX_SIZE 5024291
/*Maximum data size*/
#define DATA_SIZE 1638375

/*Push macro, implements stack operation push*/
#define PUSH(A) (stack[sp++] = A)
/*Pop macro, implements stack operation pop*/
#define POP() (stack[--sp])
/*Empty macro, implements stack operation check if empty*/
#define EMPTY() (sp == 0)
/*Full macro, implements stack operation check if full*/
#define FULL() (sp == MAX_SIZE)


/**
 * @brief die
 * @param code -> the error code
 * @param message -> the error message
 *
 * Lets the program die and emits an error message.
 */
static inline void die(int code, const char* message){
    fprintf(stderr, "%s", message);
    exit(code);
}

/** 
 * @brief An instruction struct
 *
 * Consists of operator and operand,
 * the easiest way of abstracting
 * an instruction.
 */
struct instruction{
    unsigned int operator; ///< Operator
    unsigned int operand; ///< Operand
};

/*The program as an array of structs*/
static struct instruction program[PROGRAM_SIZE];
/*The stack*/
static unsigned int stack[MAX_SIZE];
/*The stack pointer*/
static unsigned long sp = 0;

/**
 * @brief compile
 * @param fp -> the file
 *
 * Compiles a file to bytecode and saves that within program.
 */
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

/**
 * @brief execute
 *
 * Executes the bytecode within program.
 */
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

/*
 * @brief main
 * @param argc -> argument counter
 * @param argv -> argument vector
 * 
 * compiles and executes brainfuck code from a file.
 */
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
