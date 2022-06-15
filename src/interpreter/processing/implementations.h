#include "operations.h"

// Define a function to process the output of some code into destination slot
// to be used by operator implementations and runtime.c
void _recursiveprocess(opargs *args, slot *dst);

// ----- Declarations -----
void _incr(opargs *args, slot *dst);
void _decr(opargs *args, slot *dst);
void _lnot(opargs *args, slot *dst);
void _bnot(opargs *args, slot *dst);
void _mul(opargs *args, slot *dst);
void _div(opargs *args, slot *dst);
void _mod(opargs *args, slot *dst);
void _add(opargs *args, slot *dst);
void _sub(opargs *args, slot *dst);
void _let(opargs *args, slot *dst);
void _leq(opargs *args, slot *dst);
void _grt(opargs *args, slot *dst);
void _geq(opargs *args, slot *dst);
void _equ(opargs *args, slot *dst);
void _nequ(opargs *args, slot *dst);
void _band(opargs *args, slot *dst);
void _bxor(opargs *args, slot *dst);
void _bor(opargs *args, slot *dst);
void _land(opargs *args, slot *dst);
void _lor(opargs *args, slot *dst);
void _lambda(opargs *args, slot *dst);
void _ass(opargs *args, slot *dst);
void _bxorass(opargs *args, slot *dst);
void _bnotass(opargs *args, slot *dst);
void _bandass(opargs *args, slot *dst);
void _borass(opargs *args, slot *dst);
void _addass(opargs *args, slot *dst);
void _subass(opargs *args, slot *dst);
void _mulass(opargs *args, slot *dst);
void _divass(opargs *args, slot *dst);
void _modass(opargs *args, slot *dst);
void _default(opargs *args, slot *dst);
void _list(opargs *args, slot *dst);
void _end(opargs *args, slot *dst);

void _preincr(opargs *args, slot *dst);
void _predecr(opargs *args, slot *dst);
void _pos(opargs *args, slot *dst);
void _neg(opargs *args, slot *dst);

void _call(opargs *args, slot *dst);
void _index(opargs *args, slot *dst);
void _block(opargs *args, slot *dst);
void _block_end(opargs *args, slot *dst);