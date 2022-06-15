#pragma once

#include "../../memory/stack.h"
#include "../../memory/hashtable.h"
#include "../mapop.h"
#include "../bytecode.h"

// Define struct for operation function arguments
typedef struct {
    hashtable* hashtable;

    errorinfo *info; // This information will not be exact, but better than nothing

    // the bytecode to be recursively processed
    bytecode *code;
} opargs;

// Define return type and arguments of operation functions
typedef mementry *(*operation)(opargs *);

// an array of operations that can be directly indexed by the opcode from mapop.h
// mapped in operations.c
// implementations in implementations.c
operation implementationof(opcode code);