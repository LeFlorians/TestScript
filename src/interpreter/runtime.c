#include "processing/implementations.h"

#include "runtime.h"
#include "../error/error.h"



void process(bytecode *code, errorinfo *info, hashtable *memory) {
    number num;
    // allocate space for operation functions
    opargs args;

    args.info = info;
    args.code = code;

    args.hashtable = memory;

    // slot to put the result in
    slot dst;
    dst.type = EMPTY; // TODO: remove later if possible

    // process load the result into arbitrary slot 1
    // this function is defined in implementations.c
    _recursiveprocess(&args, &dst);
    
    // TODO: remove
    // DEBUG print result
    printf("Result: ");
    switch(dst.type) {
        case EMPTY:
            printf("Empty\n");
            break;
        case NUMBER:
            printf("Number (%.15llf)\n", *dst.value.number);
            break;
        case STRING:
            printf("String (%s)\n", dst.value.string);
            break;
        default:
            break;
    }

}