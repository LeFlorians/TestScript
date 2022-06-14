#include "runtime.h"

void process(bytecode code) {
    opcode *opcodeptr;
    number num;
    // allocate space for operation functions
    opargs args;
    

    // Iterate through all bytes of expressions
    while((opcodeptr = (opcode *)pop(code.expr, 1)) != NULL) {
        
        // resolve implementation of operation by opcode
        operation op = implementationof(*opcodeptr);

    }

}