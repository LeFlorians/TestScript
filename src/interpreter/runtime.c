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

    // process the given bytecode
    // this function is defined in implementations.c
    mementry *dst = _recursiveprocess(&args, 0); // just return a copy here
    
    // TODO: remove
    // DEBUG print result
    printf("Result: ");

    if(dst == NULL){
        printf("No result");
        return;
    }

    switch(dst->type) {
        case NUMBER:
            printf("Number (%g)\n", (double) *(number *)dst->value);
            break;
        case STRING:
            printf("String (%s)\n", (char *)dst->value);
            break;
        case FUNCTION:
            printf("Function\n");
            break;
        default:
            break;
    }

}