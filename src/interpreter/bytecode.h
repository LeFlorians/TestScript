#include <stdlib.h>

#include "parser.h"

/* 
    Bytecode format specification

    Each bytecode instruction is composed as follows
    (one control byte) (additional value bytes)

    The control byte looks as follows:
    0b 00000000
       || 00=A value follows, 01=This is an operation, 10=BLOCK/Scope down, 11=BLOCK_END/Scope up
       if value:
         || datatype: 00=Number, 01=String, 10=Field
           |||| The size of the value
            if number:          doesn't matter, always 64bit
            if string/field:    n * 16 = amount of bytes that follow
       if operation:
         |||||| opcode
    
    If a load/store operation is performed, additional value/addres bytes have to be set
    Their type is defines as any of: field descriptor, number value, string(number array) value
    These will be in the appropriate internal format already


    Type definitions

    Numbers:
    A number is always a 64bit double (little endian)

    Objects:
    Internally handled as hash tables with keys and values of any type

    Array:
    Internally handled as array of pointers (as size & type of values can differ)

    String:
    Internally handled as array of characters

*/

// A number's datatype
typedef union {
    double value;
    long binary;
} number;


// Definition of bytecode
typedef struct {
    void* ptr; // where the bytecode is
    size_t size; // size of the bytecode in bytes
} bytecode;

// Consumes a syntax tree (freeing it) and returns the corresponding bytecode 
bytecode *consume(stnode *root);