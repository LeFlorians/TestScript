#include "../../memory/stack.h"
#include "../../memory/hashtable.h"
#include "../mapop.h"
#include "../bytecode.h"

// Define some space to be allocated for the implementation functions
typedef struct {
    typing type; // Any of NUMBER, FIELD, STRING ARRAY, OBJECT
    union {
        char* content;
        number num;
    } value;
} slot;

// Define struct for operation function arguments
typedef struct {
    stack *stack;
    slot slot1, slot2;
    
    hashtable* hashtable;

    errorinfo *info; // This information will not be exact, but better than nothing
} opargs;

// Define return type and arguments of operation functions
typedef void (*operation)(opargs *);

// an array of operations that can be directly indexed by the opcode from mapop.h
// mapped in operations.c
// implementations in implementations.c
operation implementationof(opcode code);