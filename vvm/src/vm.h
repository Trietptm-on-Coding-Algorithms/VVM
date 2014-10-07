#ifndef VM_H
#define VM_H

#include <stdio.h>

#include "util.h"
#include "opcode.h"

/**
 * @brief vm_execute
 * @param code -> instructions
 * @param ip -> starting point
 * @param datasize -> maximum data size
 * @param length -> length of program
 */
void vm_execute(int[], int, int, int);

#endif
