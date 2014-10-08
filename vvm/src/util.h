#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief die
 * @param code -> the error code
 * @param message -> the error message
 *
 * Lets the program die and emits an error message.
 */
static inline void die(int code, const char* message){
    fprintf(stderr, "%s\n", message);
    exit(code);
}
#endif
