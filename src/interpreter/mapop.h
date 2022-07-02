#pragma once

#include <string.h>

typedef struct s_op operator;

// define opcodes
typedef enum {
    // see mapop.gperf for symbols
    // see operations.c for operation-function mappings
    // see implementations.h for declarations
    // see implementations.c for definitions

    OP_INCR, OP_DECR, OP_LNOT, OP_BNOT, OP_MUL, 
    OP_DIV, OP_MOD, OP_ADD, OP_SUB, OP_LET, 
    OP_LEQ, OP_GRT, OP_GEQ, OP_EQU, OP_NEQU, 
    OP_BAND, OP_BXOR, OP_BOR, OP_LAND, OP_LOR, 
    OP_LAMBDA, OP_ASS, OP_BXORASS, OP_BNOTASS, 
    OP_BANDASS, OP_BORASS, OP_ADDASS, OP_SUBASS, 
    OP_MULASS, OP_DIVASS, OP_MODASS, OP_DEFAULT, 
    OP_LIST, OP_END,

    // Special prefix operators
    OP_PREINCR, OP_PREDECR, OP_POS, OP_NEG,

    // Internal operators
    OP_CALL, OP_INDEX, OP_ARRAY,
    OP_BLOCK, OP_BLOCK_END

} opcode;

struct s_op {
    char *name;
    
    unsigned char opcode;

    enum {
        INFIX=0b00, POSTFIX=0b01, PREFIX=0b10,
    } position;

    enum {
        LTR=0, RTL=1,
    } associativity;

    unsigned char precedence;
};

// Function to map infix/postfix operators
operator *mapop(char *expr);

// Function to map expecially prefix operators
// This replaces ++,--,+,- with the correct values if necessary
operator *mappreop(char *expr);