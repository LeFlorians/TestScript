#include "implementations.h"

// ----- Helper functions -----

// makro to easily get the next type
#define TYPE assertExistance(pop(args->stack, 1), args)


// returns the address of the data according to the stack, given by it's type (and therefore size on stack)
// fetches field pointers
// types: NUMBER=(number *) STRING=(char *) FIELD (hashelement *)
void *resolve(char type, opargs *args) {

    switch(type) {
        case FIELD:
            // resolve key, find value in hashtable
            return find(args->hashtable, *((char **) pop(args->stack, sizeof(char**))));

        case STRING:
            return *((char **) pop(args->stack, sizeof(char**)));

        case NUMBER:
            return (number *) pop(args->stack, sizeof(number));
    }
}

// assert that a type is not null, throw an error otherwise
// still returns NULL!
char *assertExistance(void *typename, opargs *args) {
    if(typename == NULL)
        throw(EI_MISSING_ARGS, args->info);
    return (char *)typename;
}








// ----- Operator implementations -----
void _incr(opargs *args) {
    // pop one value from the stack, increment it and push it back
    char *type = TYPE;
    
    if(type == NUMBER){
        // get number address from stack, dereference it, then increment
        (*((number *)resolve(type, args->stack)))++;
    }


}

void _decr(opargs *args) {

}

void _lnot(opargs *args) {

}

void _bnot(opargs *args){

}

void _mul(opargs *args){

}

void _div(opargs *args) {

}

void _mod(opargs *args){

}

void _add(opargs *args){

}


void _sub(opargs *args){
}

void _let(opargs *args){
}

void _leq(opargs *args){
}

void _grt(opargs *args){
}

void _geq(opargs *args){
}

void _equ(opargs *args){
}

void _nequ(opargs *args){
}

void _band(opargs *args){
}

void _bxor(opargs *args){
}

void _bor(opargs *args){
}

void _land(opargs *args){
}

void _lor(opargs *args){
}

void _lambda(opargs *args){
}

void _ass(opargs *args){
}

void _bxorass(opargs *args){
}

void _bnotass(opargs *args){
}

void _bandass(opargs *args){
}

void _borass(opargs *args){
}

void _addass(opargs *args){
}

void _subass(opargs *args){
}

void _mulass(opargs *args){
}

void _divass(opargs *args){
}

void _modass(opargs *args){
}

void _default(opargs *args){
}

void _list(opargs *args){
}

void _end(opargs *args){
}


void _preincr(opargs *args){
}

void _predecr(opargs *args){
}

void _pos(opargs *args){
}

void _neg(opargs *args){
}


void _call(opargs *args){
}

void _index(opargs *args){
}

void _block(opargs *args){
}

void _block_end(opargs *args){
}
