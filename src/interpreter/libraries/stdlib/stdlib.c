#include "stdlib.h"
#include "../../processing/implementations.h"
#include "../../error/error.h"
#include "../../bytecode.h"
#include "../../memory/array.h"

#define ALLOC 1

// ----- stdlib implementations -----

// return type-string of given argument
void _type(mementry *args, mementry *dst) {
    static const char* typeNames[] = {
        "number", "string", "undefined", "array", "tuple",
        "object", "function", "cfunction", "reference", 
        "nulltkn", "bracket", "symbol", "field", "eof",
        "block", "block_end", "member", "expr"
    };

    dst->type = STRING;
    dst->value = strdup(typeNames[args->type]);
}

// print a value to the console
void _print(mementry *args, mementry *dst) {
    switch(args->type) {
        case NUMBER:
            printf("%Lg", *((number *)args->value));
            break;

        case STRING:
            printf("%s", (char *)args->value);
            break;

        // if anything else is passed, return 0
        default:
            dst->type = NUMBER;
            dst->value = malloc(sizeof(number));
            *((number *)dst->value) = (number) 0;
            return;
    }

    dst->type = NUMBER;
    dst->value = malloc(sizeof(number));
    *((number *)dst->value) = (number) 1;
    return;
}

// execute a system shell comand (sh on UNIX, cmd on Windows)
void _exec(mementry *args, mementry *dst) {
    dst->type = NUMBER;
    dst->value = malloc(sizeof(number));

    if(args->type != STRING){
        *((number *)dst->value) = (number) 0;
        return;
    }
    
    system((char*) args->value);
    
    *((number *)dst->value) = (number) 1;
}

void _if(mementry *args, mementry *dst) {
    if(args->type == TUPLE){
        array *arr = ((array *)args->value);
        mementry *fun = NULL;
        // can be (condition, if) or (condition, if, else)
        if(arr->size == 2 || arr->size == 3) {
            mementry *cond = arr->arr[0];
            // resolve field
            if(cond->type == REFERENCE) {
                cond = cond->value;
            }
            if(truth_of(cond)) {
                // perform if case
                fun = arr->arr[1];
            } else if(arr->size == 3) {
                // perform else case 
                fun = arr->arr[2];
            }
            if(fun == NULL) {
                dst->type = UNDEFINED; 
            } else {
                dst->type = fun->type;
                dst->level = fun->level;
                dst->value = fun->value;

                // free fun if synthetic, but not value
                if(fun->flags.synthetic)
                    free(fun);
            }
            return;
        }
    }
    // error during if statement
    printf("[error during execution of if-statement]\n");
}

// -----

void loadstd(hashtable *table){

    // array of all functions and their name
    static struct func_id {
        wfunction func;
        char *name;
    } functions[] = {

        // --- Array of all functions and their names in the stdlib

        { _print, "print" },
        { _type,  "type" },
        { _exec,  "exec" },
        { _if,    "if" },

        // ---
    };

    // get std table
    mementry *std = find(table, "std");
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
        mementry *dst = find(stdtable, functions[i].name); // search by value
        dst->type = CFUNCTION; // set type to c function

        cfunction *cf = malloc(sizeof(cfunction)); // allocate new cfunction
        dst->value = cf; // copy cfunction pointer

        cf->funptr = functions[i].func; // copy over function pointer
    }

}
