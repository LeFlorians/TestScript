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

    // process load the result into arbitrary slot 1
    // this function is defined in implementations.c
    _recursiveprocess(&args, &dst);
    

}