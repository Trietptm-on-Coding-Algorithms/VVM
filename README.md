Veits Virtual Machine
---------------------

A virtual machine that executes assembler-like code.

Performance
-----------

Performance is pretty okay on basic, small programs. The included factorial
function takes 0.1 second on 100000 iterations. As a scale, a Python 3.3 program
on the same machine using the code:

``
def fac(n):
    if n < 2: return 1
    else: return n * fac(n-1)
fac(12)
``

takes about 3.7 seconds on 100000 iterations.

This does not say anything about overall performance though and I am not
sure whether this small, funny test has any real value in measuring performance.
Also, Python is much more feature-rich, so you cannot compare the two at all.

DISCLAIMER: Parsing takes quite a long time, so try to write your programs as 
concise as possible.

Instruction set
---------------

Up until now, all the operations are executed on integers.

*Operations overview*:

* ENTRY - Specifies an entry point for the routine by its index.

* ADD - Adds the two items to each other that are on top of the stack
and stores the result on top of the stack.

* SUB - Subtracts the two items from each other that are on top of the
stack and stores the result on top of the stack.

* MULT - Multiplies the two items to each other that are on top of the
stack and stores the result on top of the stack.

* DIV - Performs a division operation to the two items that are on top 
of the stack and stores the result on top of the stack.

* MOD - Performs a modulo operation to the two items that are on top 
of the stack and stores the result on top of the stack.

* LT - Checks whether the item on top of the stack is greater than the
item below it and stores a boolean on top of the stack.

* EQ - Checks whether the item on top of the stack is equal to the
item below it and stores a boolean on top of the stack.

* GT - Checks whether the item on top of the stack is smaller than the
item below it and stores a boolean on top of the stack.

* LEQ - Checks whether the item on top of the stack is smaller than or 
equal to the item below it and stores a boolean on top of the stack.

* GEQ - Checks whether the item on top of the stack is greater than or 
equal to the item below it and stores a boolean on top of the stack.

* BR - Jumps to the instruction that is provided as an argument.

* BRT - Jumps to the instruction that is provided as an argument if the
value on top of the stack is TRUE.

* BRF - Jumps to the instruction that is provided as an argument if the
value on top of the stack is FALSE.

* CONST - Puts the argument provided to the operation on top of the 
stack.

* LOAD - Loads an element from any position on the stack on top of the
stack(usually used to get function arguments like this: LOAD -3).

* GLOAD - Loads an element from any position on the data segment  on 
top of the stack.

* STORE - Stores an element on the stack.

* GSTORE - Stores an element in the data segment.

* PRINT - Prints the element on top of the stack as character.

* IPRINT - Prints the element on top of the stack as integer.

* POP - Pops the element on top of the stack.

* HALT - Ends the program.

* CALL - calls a subroutine with a number of arguments.

* RET - Returns from a subroutine with a return value.

* FETCH - Fetches a value.

* INC -Increments the value on top of the stack by one(equal to ++).

* DEC -Decrements the value on top of the stack by one(equal to --).

