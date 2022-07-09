#include "stdlib.h"
#include "../dlibs/libraries.h"

#include "../bytecode.h"

#define ALLOC 1

// ----- stdlib implementations -----
void _print(mementry *args, mementry *dst) {
    
    // print the number in args
    printf("Printing number: %Lg\n", *((number *)args->value));
    
    dst->type = args->type;
    dst->value = args->value;
}

// -----

void loadstd(hashtable *table){

    // array of all functions and their name
    static struct func_id {
        wrapfunction func;
        char *name;
    } functions[] = {

        // --- Array of all functions and their names in the stdlib
        
        { _print, "print" },
    
        // ---
    };
    
    // register all functions
    for(unsigned i = 0; i < sizeof(functions) / sizeof(struct func_id); i++) {
        mementry *dst = find(table, functions[i].name, ALLOC); // search by value
        dst->type = CFUNCTION; // set type to c function
        
        cfunction *cfunc = malloc(sizeof(cfunction)); // allocate new cfunction
        dst->value = cfunc; // copy cfunction pointer
        
        cfunc->wrapped = 1; // make sure this is recognized as wrapfunction
        cfunc->funptr = functions[i].func; // copy over function pointer
    }

}