#include "processing/operations.h"

/*
    The runtime type provides a complete environment for code to be run in
*/

// type of function
typedef struct {
    bytecode *code; // function code stack

    hashtable *defaults; // object containing default values
} function;

void process(bytecode *code, errorinfo *info, hashtable *memory);