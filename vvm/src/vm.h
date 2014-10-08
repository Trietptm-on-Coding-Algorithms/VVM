#ifndef VM_H
#define VM_H

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "util.h"
#include "opcode.h"

typedef struct{
    unsigned long length;
    int entrypoint;
    int* code;
} program;

/**
 * @brief vm_execute
 * @param code -> instructions
 * @param ip -> starting point
 * @param datasize -> maximum data size
 * @param length -> length of program
 */
void vm_execute(int[], int, int, unsigned long);

/**
 * @brief vm_parse
 * @param file -> filename
 */
program vm_parse(char*);

char** str_split(char*, const char);

#endif
