#include "implementations.h"

// ----- Helper functions -----

#define COPY 0
#define REFERENCE 1

mementry *_recursiveprocess(opargs *args, char return_reference) {
    opcode *opcodeptr = (opcode *)pop(args->code, 1);

    // check if NULL
    if(opcodeptr == NULL){
        // return NULL
        return NULL;
    }

    mementry *ret;

    // use unsigned char, as opcode iw in range
    unsigned char opcode = *opcodeptr;

    // check if its an opcode
    if((opcode & (char)128) != 0) {
        // apply this operator
        opcode = (opcode & (char)127); // convert opcode back to true opcode (see bytecode.h)

        // perform the found operation
        return implementationof(opcode)(args);

    } else {
        // TODO: handle other cases
        // ! because the bytecode is static and may be cached, values from here must be copied, not referenced
        switch (opcode) {
            case NUMBER:
                // allocate ret
                ret = malloc(sizeof(mementry));

                // copy value instead of pointing to it
                ret->value =  malloc(sizeof(number));
                *(number *)ret->value = *((number *)pop(args->code, sizeof(number)));
                
                // set number type
                // typing can be cast, as the used values are equal
                ret->type = NUMBER;
                
                break;

            case STRING:
                // allocate ret
                ret = malloc(sizeof(mementry));

                // copy the entire string
                ret->value = strdup(*((char **) pop(args->code, sizeof(char**))));

                // set string type
                ret->type = STRING;
                break;

            case FIELD:
                // If a field is returned, really just put its value into dst
                // pretending the field is an actual value node, for simplicity

                hashelement *res = find(args->hashtable, return_reference, *((char **) pop(args->code, sizeof(char**))));

                // allocate and return default value
                if(res == NULL){
                    // return a default value
                    mementry *ret = malloc(sizeof(mementry));
                    ret->type = NUMBER;
                    ret->value = malloc(sizeof(number));
                    *(number *)ret->value = (number) 0;

                    // break from switch
                    break;
                }

                // set default value
                if(res->valueptr == NULL) {
                    // TODO: search for predefined default value here

                    // allocate mementry
                    ret = malloc(sizeof(mementry));

                    // set type
                    ret->type = NUMBER;

                    // allocate and set default number
                    ret->value = malloc(sizeof(number));
                    *(number *)ret->value = (number) 0;

                    // assign to hashelement
                    res->valueptr = ret;

                } else {
                    if(return_reference) {
                        // return a reference
                        ret = res->valueptr;

                    } else {
                        // return a copy
                        ret = malloc(sizeof(mementry));

                        // copy type
                        ret->type = res->valueptr->type;

                        // copy contents
                        switch(ret->type) {
                            case NUMBER:
                                ret->value = malloc(sizeof(number));
                                *(number *)ret->value = *(number *)res->valueptr->value;
                                break;
                            case STRING:
                                ret->value = strdup((char *) res->valueptr->value);
                                break;
                        }

                    }
                }

                break;
        }
    }
    return ret;
}

// assert that a type is not null
// if type is null, returns -1
typing assertExistance(mementry **typename, opargs *args) {
    if(typename == NULL) {
        throw(EI_MISSING_ARGS, args->info);
        return -1;
    }
    return *((typing *)typename);
}






// ----- Operator implementations -----


mementry *_incr(opargs *args) {
    mementry *dst = _recursiveprocess(args, COPY); // Load only operand into mementry

    if(dst->type == NUMBER) {
        (*(number*)dst->value)++;
    } else
        throw(EI_INVALID_COMBINATION, args->info);
    return dst;
}

mementry *_decr(opargs *args) {
    mementry *dst = _recursiveprocess(args, COPY); // Load only operand into mementry

    if(dst->type == NUMBER) {
        (*(number*)dst->value)--;
    } else
        throw(EI_INVALID_COMBINATION, args->info);
    return dst;
}

mementry *_lnot(opargs *args) {
    mementry *dst = _recursiveprocess(args, COPY); // Load only operand into mementry

    if(dst->type == NUMBER) {
        *(number *)dst->value = !*(number *)dst->value;
    } else
        throw(EI_INVALID_COMBINATION, args->info);
    return dst;
}

mementry *_bnot(opargs *args) {
    mementry *dst = _recursiveprocess(args, COPY); // Load only operand into mementry

    if(dst->type == NUMBER) {
        // ? may round
        *(number *)dst->value = -*(number *)dst->value - 1;
    } else
        throw(EI_INVALID_COMBINATION, args->info);
    return dst;
}

mementry *_mul(opargs *args){
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value *= *(number *)tmp->value;
            }
            break;
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_div(opargs *args) {
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value /= *(number *)tmp->value;
            }
            break;
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_mod(opargs *args){

}

mementry *_add(opargs *args){
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value += *(number *)tmp->value;
            }
            break;
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_sub(opargs *args){
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value -= *(number *)tmp->value;
            }
            break;
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_let(opargs *args){
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value *= *(number *)tmp->value;
            }
            break;
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_leq(opargs *args){
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value = *(number *)dst->value  <= *(number *)dst->value;
            }
            break;
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_grt(opargs *args){
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value = *(number *)dst->value > *(number *)dst->value;
            }
            break;
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_geq(opargs *args){

}

mementry *_equ(opargs *args){

}

mementry *_nequ(opargs *args){
}

mementry *_band(opargs *args){
}

mementry *_bxor(opargs *args){
}

mementry *_bor(opargs *args){
}

mementry *_land(opargs *args){
}

mementry *_lor(opargs *args){
}

mementry *_lambda(opargs *args){
}

mementry *_ass(opargs *args){
    mementry *dst = _recursiveprocess(args, REFERENCE); // Load left operand into dst
    mementry *src = _recursiveprocess(args, COPY); // Load right operand into src

    // copy type
    dst->type = src->type;

    // free original dst value
    free(dst->value);

    // create value reference
    dst->value = src->value;

    // free src
    free(src);

    return dst;
}

mementry *_bxorass(opargs *args){
}

mementry *_bnotass(opargs *args){
}

mementry *_bandass(opargs *args){
}

mementry *_borass(opargs *args){
}

mementry *_addass(opargs *args){
}

mementry *_subass(opargs *args){
}

mementry *_mulass(opargs *args){
}

mementry *_divass(opargs *args){
}

mementry *_modass(opargs *args){
}

mementry *_default(opargs *args){
}

mementry *_list(opargs *args){
}

mementry *_end(opargs *args){
}


mementry *_preincr(opargs *args){
}

mementry *_predecr(opargs *args){
}

mementry *_pos(opargs *args){
}

mementry *_neg(opargs *args){
}


mementry *_call(opargs *args){
}

mementry *_index(opargs *args){
}

mementry *_block(opargs *args){
}

mementry *_block_end(opargs *args){
}
