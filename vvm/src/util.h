#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include <stdlib.h>

#define LENGTH(x) (sizeof(x) / sizeof(x[0]))

static inline void die(int code, const char* message){
    fprintf(stderr, "%s", message);
    exit(code);
}
#endif
