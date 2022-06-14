#include "processing/implementations.h"

#include "runtime.h"
#include "../error/error.h"



void process(bytecode *code, errorinfo *info) {
    number num;
    // allocate space for operation functions
    opargs args;

    args.info = info;
    args.code = code;

    // TODO: choose better hashtable width
    args.hashtable = create_hashtable(32);

    // process load the result into arbitrary slot 1
    // this function is defined in implementations.c
    _recursiveprocess(&args, &args.slot1);
    

}