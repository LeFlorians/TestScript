#include <stdlib.h>

#include "parser.h"

/* 
    Bytecode format specification

    Each bytecode instruction is composed as follows
    (one control byte) (additional value bytes)

    The control byte looks as follows:
    0b 00000000
       || 00=This is an operation, 01=A value follows, 10=BLOCK/Scope down, 11=BLOCK_END/Scope up
       if value: (a pointer sizeof(void *) to the data follows)
         || datatype: 00=Number (64bit float), 01=String (null-terminated), 10=Field (null-terminated)
           |||| unused 
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
typedef long double number;

// bytecode is just a stack-pointer
typedef stack *bytecode;

// Consumes a syntax tree (freeing it) and returns the corresponding bytecode 
bytecode consume(stnode *root);