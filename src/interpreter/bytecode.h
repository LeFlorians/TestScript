#pragma once

#include <stdlib.h>

#include "parser.h"

/* 
    Bytecode format specification

    Bytecode is made up of two data blobs:
      - one for Data
        => After a -value- there follows always a control byte
          - value can be: number for number, char* for string, char* for field
          - control byte can be (see tokenizer.h)
            NUMBER: number
            STRING: string
            FIELD: field
            ARRAY: internal array
            OBJECT: internal object
      - one for Expressions, with zero, one or two arguments
        - if expression; (its opcode) | 128
        - if block: OP_BLOCK | 128, if block_end: OP_BLOCK_END | 128
    
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

typedef stack bytecode;

// Consumes a syntax tree (freeing it) and a hashtable and returns the corresponding bytecode 
bytecode *consume(stnode *root);
