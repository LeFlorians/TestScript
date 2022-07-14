#include "../processing/implementations.h"
#include "mapop.h"

// This file only creates mappings between opcodes and corresponding operation functions
// For implementations, see implementations.c

// Function to return function that corresponds to given opcode
operation implementationof(opcode code) {
    static operation implementations[] = {

            // ----- Mappings -----

            [OP_INCR]      = _incr,
            [OP_DECR]      = _decr,
            [OP_LNOT]      = _lnot,
            [OP_BNOT]      = _bnot,
            [OP_MUL]       = _mul,
            [OP_DIV]       = _div,
            [OP_MOD]       = _mod,
            [OP_ADD]       = _add,
            [OP_SUB]       = _sub,
            [OP_LET]       = _let,
            [OP_LEQ]       = _leq,
            [OP_GRT]       = _grt,
            [OP_GEQ]       = _geq,
            [OP_EQU]       = _equ,
            [OP_NEQU]      = _nequ,
            [OP_BAND]      = _band,
            [OP_BXOR]      = _bxor,
            [OP_BOR]       = _bor,
            [OP_LAND]      = _land,
            [OP_LOR]       = _lor,
            [OP_LAMBDA]    = _lambda,
            [OP_ASS]       = _ass,
            [OP_HARDSET]   = _hardset,
            [OP_BXORASS]   = _bxorass,
            [OP_BNOTASS]   = _bnotass,
            [OP_BANDASS]   = _bandass,
            [OP_BORASS]    = _borass,
            [OP_ADDASS]    = _bandass,
            [OP_SUBASS]    = _subass,
            [OP_MULASS]    = _mulass,
            [OP_DIVASS]    = _divass,
            [OP_MODASS]    = _modass,
            [OP_LIST]      = _list,
            [OP_END]       = _end,

            [OP_PREINCR]   = _preincr,
            [OP_PREDECR]   = _predecr,
            [OP_POS]       = _pos,
            [OP_NEG]       = _neg,

            [OP_CALL]      = _call,
            [OP_INDEX]     = _index,
            [OP_ARRAY]     = _array,
            [OP_BLOCK]     = _block,
            [OP_BLOCK_END] = _block_end,
            
    };
    return implementations[code];
}


