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
        implementationof(opcode)(args);

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
                    ret->type = NUMBER;
                    //allocate number and set default value
                    ret->value = malloc(sizeof(number));
                    *((number *) ret->value) = (number) 0;

                }
                dst->value.field = ret;
                break;
        }
    }
}

// assert that a type is not null
// if type is null, returns -1
typing assertExistance(void *typename, opargs *args) {
    if(typename == NULL) {
        throw(EI_MISSING_ARGS, args->info);
        return -1;
    }
    return *((typing *)typename);
}








// ----- Operator implementations -----
void _incr(opargs *args) {
    slot slt;
    _recursiveprocess(args, &slt); // Load only operand into slot

    if(slt.type == NUMBER){
        (*slt.value.number)++; // Increment if number
    } else if(slt.type == FIELD && slt.value.field->type == NUMBER){
        (*((number *)slt.value.field->value))++; // Increment if it's field containing a number
    } else {
        slt.type == EMPTY;
        throw(EI_INVALID_COMBINATION, args->info);
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
