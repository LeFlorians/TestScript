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
        // ! because the bytecode is static and may be cached, values from here must be copied, not referenced
        switch (opcode) {
            case NUMBER:
                // copy value instead of pointing to it
                dst->value.number = (number *) malloc(sizeof(number));
                *dst->value.number = *((number *)pop(args->code, sizeof(number)));
                break;

            case STRING:
                // copy the entire string
                dst->value.string = strdup(*((char **) pop(args->code, sizeof(char**))));
                break;

            case FIELD:
                // If a field is returned, really just put its value into dst
                // pretending the field is an actual value node, for simplicity

                hashelement *res = find(args->hashtable, *((char **) pop(args->code, sizeof(char**))));

                // set default value
                if(res->type == EMPTY) {
                    // TODO: search for predefined default value here

                    // set both types
                    dst->type = res->type = NUMBER;

                    // allocate number and set default value
                    // this is a reference, not a copy, because it exists in the hashtable!
                    dst->value.number = res->value = malloc(sizeof(number));
                    *(dst->value.number) = (number) 0;
                } else {
                    dst->type = res->type;
                    // set one of the value pointers
                    dst->value.number = res->value;
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
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
}

void _decr(opargs *args, slot *dst) {
    _recursiveprocess(args, dst); // Load only operand into slot

    switch (dst->type) {
        case NUMBER:
            (*(dst->value.number))--; // Decrement if number
            return;
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
}

void _lnot(opargs *args, slot *dst) {
    _recursiveprocess(args, dst); // Load only operand into slot

    switch (dst->type) {
        case NUMBER:
            (*(dst->value.number)) = !(*(dst->value.number)); // Locigal NOT if number
            return;
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
}

void _bnot(opargs *args, slot *dst){
    _recursiveprocess(args, dst); // Load only operand into slot

    switch (dst->type) {
        case NUMBER:
            // ? may round value like JS
            (*(dst->value.number)) = -(*(dst->value.number)) - 1; // Locigal NOT if number
            return;
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
}

void _mul(opargs *args, slot *dst){
    slot tmp; // temporary slot
    _recursiveprocess(args, dst); // Load left operand into dst slot
    _recursiveprocess(args, &tmp); // Load right operand into tmp slot

    switch (dst->type) {
        case NUMBER:
            if(tmp.type == NUMBER){
                (*(dst->value.number)) *= *tmp.value.number; // Multiply two numbers
                return;
            }
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
}

void _div(opargs *args, slot *dst) {
    slot tmp; // temporary slot
    _recursiveprocess(args, dst); // Load left operand into dst slot
    _recursiveprocess(args, &tmp); // Load right operand into tmp slot

    switch (dst->type) {
        case NUMBER:
            if(tmp.type == NUMBER){
                (*(dst->value.number)) /= *tmp.value.number;
                return;
            }
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
}

void _mod(opargs *args, slot *dst){

}

void _add(opargs *args, slot *dst){
    slot tmp; // temporary slot
    _recursiveprocess(args, dst); // Load left operand into dst slot
    _recursiveprocess(args, &tmp); // Load right operand into tmp slot

    switch (dst->type) {
        case NUMBER:
            if(tmp.type == NUMBER){
                (*(dst->value.number)) += *tmp.value.number;
                return;
            }
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
}

void _sub(opargs *args, slot *dst){
        slot tmp; // temporary slot
    _recursiveprocess(args, dst); // Load left operand into dst slot
    _recursiveprocess(args, &tmp); // Load right operand into tmp slot

    switch (dst->type) {
        case NUMBER:
            if(tmp.type == NUMBER){
                (*(dst->value.number)) += *tmp.value.number;
                return;
            }
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
}

void _let(opargs *args, slot *dst){
    slot tmp; // temporary slot
    _recursiveprocess(args, dst); // Load left operand into dst slot
    _recursiveprocess(args, &tmp); // Load right operand into tmp slot

    switch (dst->type) {
        case NUMBER:
            if(tmp.type == NUMBER){
                (*(dst->value.number)) = (*(dst->value.number)) < *tmp.value.number;
                return;
            }
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
}

void _leq(opargs *args, slot *dst){
    slot tmp; // temporary slot
    _recursiveprocess(args, dst); // Load left operand into dst slot
    _recursiveprocess(args, &tmp); // Load right operand into tmp slot

    switch (dst->type) {
        case NUMBER:
            if(tmp.type == NUMBER){
                (*(dst->value.number)) = (*(dst->value.number)) <= *tmp.value.number;
                return;
            }
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
}

void _grt(opargs *args, slot *dst){
    slot tmp; // temporary slot
    _recursiveprocess(args, dst); // Load left operand into dst slot
    _recursiveprocess(args, &tmp); // Load right operand into tmp slot

    switch (dst->type) {
        case NUMBER:
            if(tmp.type == NUMBER){
                (*(dst->value.number)) = (*(dst->value.number)) > *tmp.value.number;
                return;
            }
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
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
    slot tmp; // temporary slot
    _recursiveprocess(args, dst); // Load left operand into dst slot
    _recursiveprocess(args, &tmp); // Load right operand into tmp slot

    switch (dst->type) {
        case NUMBER:
            if(tmp.type == NUMBER){
                // Copy the result from the stack
                *dst->value.number = *tmp.value.number;
                return;
            }
    }

    dst->type = EMPTY;
    throw(EI_INVALID_COMBINATION, args->info);
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
