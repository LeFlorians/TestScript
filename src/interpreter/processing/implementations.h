#include "operations.h"

// Define a function to process the output of some code into destination slot
// to be used by operator implementations and runtime.c
void _recursiveprocess(opargs *args, slot *dst);

// ----- Declarations -----
void _incr(opargs *args);
void _decr(opargs *args);
void _lnot(opargs *args);
void _bnot(opargs *args);
void _mul(opargs *args);
void _div(opargs *args);
void _mod(opargs *args);
void _add(opargs *args);
void _sub(opargs *args);
void _let(opargs *args);
void _leq(opargs *args);
void _grt(opargs *args);
void _geq(opargs *args);
void _equ(opargs *args);
void _nequ(opargs *args);
void _band(opargs *args);
void _bxor(opargs *args);
void _bor(opargs *args);
void _land(opargs *args);
void _lor(opargs *args);
void _lambda(opargs *args);
void _ass(opargs *args);
void _bxorass(opargs *args);
void _bnotass(opargs *args);
void _bandass(opargs *args);
void _borass(opargs *args);
void _addass(opargs *args);
void _subass(opargs *args);
void _mulass(opargs *args);
void _divass(opargs *args);
void _modass(opargs *args);
void _default(opargs *args);
void _list(opargs *args);
void _end(opargs *args);

void _preincr(opargs *args);
void _predecr(opargs *args);
void _pos(opargs *args);
void _neg(opargs *args);

void _call(opargs *args);
void _index(opargs *args);
void _block(opargs *args);
void _block_end(opargs *args);