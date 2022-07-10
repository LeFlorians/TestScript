#include "processing/implementations.h"

#include "runtime.h"
#include "../error/error.h"

#include "../memory/array.h"

// private functions for output formatting

// TODO: make thread safe
int _indent = 0;
void _print_object(tableentry *entry);

void _print_value(mementry *dst) {


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




// Actual function processing the bytecode and printing the result
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
    printf("Result:\n");
    _indent = 0;
    _print_value(dst);
}