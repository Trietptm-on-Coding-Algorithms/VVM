#include "vm.h"
#include "util.h"

/**
 * @brief main
 * argc -> the argument counter
 * argv -> the argument vector
 *
 * executes code in my dsl.
 */
int main(int argc, char** argv){
    program prog;
    if(argc != 2)
        die(127, "Please specify exactly one file to execute.");
    prog = vm_compile(argv[1]);
    vm_execute(prog.code, prog.entrypoint, 0, prog.length);
    free(prog.code);
    return 0;
}
