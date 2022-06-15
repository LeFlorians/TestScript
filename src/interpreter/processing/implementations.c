#include "implementations.h"

// ----- Helper functions -----

void _recursiveprocess(opargs *args, slot *dst) {

    opcode *opcodeptr = (opcode *)pop(args->code, 1);

    // check if NULL
    if(opcodeptr == NULL){
        // clear the dst type
        dst->type = EMPTY;
        return;
    }

    // use unsigned char, as opcode iw in range
    unsigned char opcode = *opcodeptr;

    // check if its an opcode
    if((opcode & (char)128) != 0) {
        // apply this operator
        opcode = (opcode & (char)127); // convert opcode back to true opcode (see bytecode.h)

        // perform the found operation
        implementationof(opcode)(args, dst);

    } else {
        // typing can be cast, as the used values are equal
        dst->type = (typing) opcode;

        // TODO: handle other cases
        switch (opcode) {
            case NUMBER:
                dst->value.number = (number *) pop(args->code, sizeof(number));
                break;

            case STRING:
                dst->value.string = *((char **) pop(args->code, sizeof(char**)));
                break;

            case FIELD:
                hashelement *ret = find(args->hashtable, *((char **) pop(args->code, sizeof(char**))));

                // set default value
                if(ret->type == EMPTY) {
                    // TODO: search for predefined default value here

                    dst->type = NUMBER;
                    //allocate number and set default value
                    dst->value.number = (number *) malloc(sizeof(number));
                    *((number *) dst->value.number) = (number) 0;
                } else {
                    dst->type = ret->type;
                    // set one of the value pointers
                    dst->value.number = ret->value;
                }
                break;
        }
    }
}

// assert that a type is not null
// if type is null, returns -1
typing assertExistance(void *typename, opargs *args, slot *dst) {
    if(typename == NULL) {
        throw(EI_MISSING_ARGS, args->info);
        return -1;
    }
    return *((typing *)typename);
}








// ----- Operator implementations -----
void _incr(opargs *args, slot *dst) {
    _recursiveprocess(args, dst); // Load only operand into slot

    switch (dst->type) {
        case NUMBER:
            (*(dst->value.number))++; // Increment if number
            return;

        default: break;
    }

    dst->type == EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
    
}

void _decr(opargs *args, slot *dst) {

}

void _lnot(opargs *args, slot *dst) {

}

void _bnot(opargs *args, slot *dst){

}

void _mul(opargs *args, slot *dst){

}

void _div(opargs *args, slot *dst) {

}

void _mod(opargs *args, slot *dst){

}

void _add(opargs *args, slot *dst){

}


void _sub(opargs *args, slot *dst){
}

void _let(opargs *args, slot *dst){
}

void _leq(opargs *args, slot *dst){
}

void _grt(opargs *args, slot *dst){
}

void _geq(opargs *args, slot *dst){
}

void _equ(opargs *args, slot *dst){
}

void _nequ(opargs *args, slot *dst){
}

void _band(opargs *args, slot *dst){
}

void _bxor(opargs *args, slot *dst){
}

void _bor(opargs *args, slot *dst){
}

void _land(opargs *args, slot *dst){
}

void _lor(opargs *args, slot *dst){
}

void _lambda(opargs *args, slot *dst){
}

void _ass(opargs *args, slot *dst){
}

void _bxorass(opargs *args, slot *dst){
}

void _bnotass(opargs *args, slot *dst){
}

void _bandass(opargs *args, slot *dst){
}

void _borass(opargs *args, slot *dst){
}

void _addass(opargs *args, slot *dst){
}

void _subass(opargs *args, slot *dst){
}

void _mulass(opargs *args, slot *dst){
}

void _divass(opargs *args, slot *dst){
}

void _modass(opargs *args, slot *dst){
}

void _default(opargs *args, slot *dst){
}

void _list(opargs *args, slot *dst){
}

void _end(opargs *args, slot *dst){
}


void _preincr(opargs *args, slot *dst){
}

void _predecr(opargs *args, slot *dst){
}

void _pos(opargs *args, slot *dst){
}

void _neg(opargs *args, slot *dst){
}


void _call(opargs *args, slot *dst){
}

void _index(opargs *args, slot *dst){
}

void _block(opargs *args, slot *dst){
}

void _block_end(opargs *args, slot *dst){
}
