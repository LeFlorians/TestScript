#include <stdlib.h>

#include "parser.h"

/* 
    Bytecode format specification

    Each bytecode instruction is composed as follows
    (one control byte) (additional value bytes)

    The control byte looks as follows:
    0b 00000000
       | Target register (either left or right)
        | set for operation, unset for load/store
            if set:
         |||||| the opcode as defined in mapop.h
            if unset:
         | set for save, unset for load
          || the datatype to be read // TODO: specify datatypes
            ||| how many bites to read, any of: 2,4,8,16,32,64,128,256
    
    If a load/store operation is performed, additional value/addres bytes have to be set
    Their type is defines as any of: field descriptor, number value, string(number array) value
    These will be in the appropriate internal format already


    Type definitions

    Numbers:
    A number is always a 64bit double (big endian)

    Objects:
    Internally handled as hash tables with keys and values of any type

    Array:
    Internally handled as array of pointers (as size & type of values can differ)

    String:
    Internally handled as array of characters

*/


// Definition of bytecode
typedef struct {
    void* ptr; // where the bytecode is
    size_t size; // size of the bytecode in bytes
} bytecode;

// Consumes a syntax tree (freeing it) and returns the corresponding bytecode 
bytecode *consume(stnode *root);