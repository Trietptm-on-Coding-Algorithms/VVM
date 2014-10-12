#ifndef VM_H
#define VM_H

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "util.h"
#include "opcode.h"

#define MAX_SIZE 4096
#define TRUE 1
#define FALSE 0
#ifdef DEBUG_ON
  #define DEBUG TRUE
#else
  #define DEBUG FALSE
#endif

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

/**
 * @brief str_split
 * @param a_str -> the input string
 * @param a_delim -> the delimiter
 * @param elemc -> the counter of elements
 *
 */
char** str_split(char*, const char, unsigned int*);

#endif
