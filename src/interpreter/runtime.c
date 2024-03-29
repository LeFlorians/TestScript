#include "processing/implementations.h"

#include "runtime.h"
#include "error/error.h"

#include "memory/array.h"

#if DEBUG
// private functions for output formatting

// TODO: make thread safe
int _indent = 0;
void _print_object(tableentry *entry);

void _print_value(mementry *dst) {

    repeat:

    if(dst == NULL){
        printf("undefined\n");
        return;
    }

    switch(dst->type) {
        case NUMBER:
            printf("%g", (double) *(number *)dst->value);
            break;
        case STRING:
            printf("'%s'",  (char *)dst->value);
            break;
        case CFUNCTION:
        case FUNCTION:
            printf("Function");
            break;
        case TUPLE:
            printf("Tuple(%lu)", ((array *)dst->value)->size);
            break;
        case ARRAY:
            printf("Array(%lu)", ((array *)dst->value)->size);  
            break;
        case OBJECT:
            printf("{\n");
            // print out the object
            walk_table((hashtable *)dst->value, _print_object);
            printf("%*s}", _indent, "");
            break;
        case UNDEFINED:
            printf("undefined");
            break;
        case REFERENCE:
            printf("Reference to: ");
            dst = dst->value;
            goto repeat;
        default:
            break;
    }
    printf("\n");
}

void _print_object(tableentry *entry) {
    _indent += 2;
    printf("%*s%s: ", _indent, "", entry->key);
    _print_value(entry->entry);
    _indent -= 2;
}
#endif


// Actual function processing the bytecode and printing the result
void process(bytecode *code, errorinfo *info) {
    // allocate space for operation functions
    opargs args;

    args.info = info;
    args.code = code;

    // set the stack offset
    args.offset = args.code->elements;

    // process the given bytecode
    // this function is defined in implementations.c
    mementry *dst = _recursiveprocess(&args, 0); // just return a copy here
    
    #if DEBUG
    // DEBUG print result
    printf("Result:\n");
    _indent = 0;
    _print_value(dst);
    #endif

    _free_synth(dst, 1);
}
