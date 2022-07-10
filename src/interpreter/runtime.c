#include "processing/implementations.h"

#include "runtime.h"
#include "../error/error.h"

#include "../memory/array.h"

void process(bytecode *code, errorinfo *info, hashtable *memory) {
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
        printf("No result\n");
        return;
    }

    switch(dst->type) {
        case NUMBER:
            printf("Number (%g)", (double) *(number *)dst->value);
            break;
        case STRING:
            printf("String (%s)", (char *)dst->value);
            break;
        case FUNCTION:
            printf("Function");
            break;
        case TUPLE:
            printf("Tuple of size %lu", ((array *)dst->value)->size);
        case ARRAY:
            printf("Array of size %lu", ((array *)dst->value)->size);  
        default:
            break;
    }
    printf("\n");

}