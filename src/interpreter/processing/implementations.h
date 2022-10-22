#pragma once

#include "../mappings/operations.h"

// Define a function to process the output of some code into destination mementry
// to be used by operator implementations and runtime.c
// @param return_reference if set,
// a reference from hashtable is returned (and element is allocated)
//  else, a copy is returned
mementry *_recursiveprocess(opargs *args, char return_reference);
void _free_synth(mementry *entry);
char truth_of(mementry *entry);

mementry *call(mementry *fun, mementry *params, errorinfo *info);

// ----- Declarations -----
mementry *_incr(opargs *args);
mementry *_decr(opargs *args);
mementry *_lnot(opargs *args);
mementry *_bnot(opargs *args);
mementry *_mul(opargs *args);
mementry *_div(opargs *args);
mementry *_mod(opargs *args);
mementry *_add(opargs *args);
mementry *_sub(opargs *args);
mementry *_let(opargs *args);
mementry *_leq(opargs *args);
mementry *_grt(opargs *args);
mementry *_geq(opargs *args);
mementry *_equ(opargs *args);
mementry *_nequ(opargs *args);
mementry *_band(opargs *args);
mementry *_bxor(opargs *args);
mementry *_bor(opargs *args);
mementry *_land(opargs *args);
mementry *_lor(opargs *args);
mementry *_lambda(opargs *args);
mementry *_ass(opargs *args);
mementry *_hardset(opargs *args);
mementry *_list(opargs *args);
mementry *_end(opargs *args);

mementry *_preincr(opargs *args);
mementry *_predecr(opargs *args);
mementry *_pos(opargs *args);
mementry *_neg(opargs *args);

mementry *_args(opargs *args);
mementry *_call(opargs *args);
mementry *_index(opargs *args);
mementry *_array(opargs *args);
mementry *_block(opargs *args);
mementry *_block_end(opargs *args);
