#include "vm.h"

/**
 * @brief main
 * argc -> the argument counter
 * argv -> the argument vector
 *
 * executes code in my dsl.
 */
int main(int argc, char** argv){
    if(argc != 2)
        die(127, "Please specify exactly one file to execute.");
    program prog = vm_parse(argv[1]);
    vm_execute(prog.code, prog.entrypoint, 0, prog.length);
    return 0;
}
