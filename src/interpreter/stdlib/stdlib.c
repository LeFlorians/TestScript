#include "stdlib.h"
#include "../dlibs/libraries.h"

#include "../bytecode.h"

#define ALLOC 1

// ----- stdlib implementations -----

// print a value to the console
void _print(mementry *args, mementry *dst) {
    dst->type = NUMBER;
    dst->value = malloc(sizeof(number));

    if(args->type != NUMBER){
        *((number *)dst->value) = (number) 0;
        return dst;
    }

    // print the number in args
    printf("Printing number: %Lg\n", *((number *)args->value));
    

    *((number *)dst->value) = (number) 1;
    return dst;
}

// execute a system shell comand (sh on UNIX, cmd on Windows)
void _exec(mementry *args, mementry *dst) {
    dst->type = NUMBER;
    dst->value = malloc(sizeof(number));

    if(args->type != STRING){
        *((number *)dst->value) = (number) 0;
        return dst;
    }
    
    system((char*) args->value);
    
    *((number *)dst->value) = (number) 1;
    return dst;
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
        { _exec,  "unsafe.exec" },

        // ---
    };

    // get std table
    mementry *std = find(table, "std", ALLOC);
    if(std->type == UNDEFINED) {
        std->value = create_hashtable(8, 4);
        std->type = OBJECT;
    }

    if(std->type != OBJECT) {
        // TODO: can't inject stdlib, throw error
        return;
    }

    hashtable *stdtable = (hashtable *)std->value;

    // register all functions
    for(unsigned i = 0; i < sizeof(functions) / sizeof(struct func_id); i++) {
        mementry *dst = find(stdtable, functions[i].name, ALLOC); // search by value
        dst->type = CFUNCTION; // set type to c function

        cfunction *cfunc = malloc(sizeof(cfunction)); // allocate new cfunction
        dst->value = cfunc; // copy cfunction pointer

        cfunc->wrapped = 1; // make sure this is recognized as wrapfunction
        cfunc->funptr = functions[i].func; // copy over function pointer
    }

}