#include "implementations.h"
#include "../memory/array.h"
#include "../libraries/libraries.h" 

// ----- Helper functions -----

// flags for the processing function
#define COPY 0
#define REFERENCE 1
#define RAW 2 // to return raw code
#define DIRECT 2 // to allocate a field in top-scope (if not field, same as reference)

#define DEFAULT_ARRAY_SIZE 8

// @param flags one of REFERENCE|COPY, defines whether space will be allocated in hashtable
//  and if a reference or copy of the value is returned
mementry *_recursiveprocess(opargs *args, char flags) {
    opcode *opcodeptr = (opcode *)pop(args->code, 1);

    // check if NULL
    if(opcodeptr == NULL){
        // return NULL
        return NULL;
    }

    mementry *ret = NULL;

    // use unsigned char, as opcode iw in range
    unsigned char opcode = *opcodeptr;

    // check if its an opcode
    if((opcode & (unsigned char)128) != 0) {
        // apply this operator
        opcode = (opcode & (unsigned char)127); // convert opcode back to true opcode (see bytecode.h)

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

            case CODE:
                // allocate ret
                ret = malloc(sizeof(mementry));

                // copy function pointer
                ret->value = *(bytecode **)pop(args->code, sizeof(bytecode *));

                // set string type
                ret->type = CODE;

                break;

            case FIELD:
                // If a field is returned, just put its value into dst
                // pretending the field is an actual value node, for simplicity
                mementry *res = NULL;
                hashtable *table = args->hashtable;
                char *key = *((char **) pop(args->code, sizeof(char**)));

                // if direct, don't search parent tables
                if(flags != DIRECT)
                    while(res == NULL && table != NULL) {
                        // TODO: save time when not found and allocating
                        // TODO: flag to only search top-level table
                        // try finding it, do not yet allocate anything
                        res = find(table, key, 0);

                        // walk up
                        table = table->parent;
                    }
                // if not found, allocate it in the highest table if requested
                if(res == NULL){
                    if(flags) {
                        // this time, allocate it
                        res = find(args->hashtable, key, 1);
                    } else {
                        // allocate a default value
                        // return a default value constant
                        ret = malloc(sizeof(mementry));
                        ret->type = NUMBER;
                        ret->value = malloc(sizeof(number));
                        *(number *)ret->value = (number) 0;

                        // break from switch
                        break;
                    }
                }

                if(flags != COPY) {
                    // return a reference (do not free references in implementations!)
                    ret = res;
                } else {
                    // return a copy
                    ret = malloc(sizeof(mementry));

                    // copy type
                    ret->type = res->type;

                    // copy contents (not just pointers)
                    switch(ret->type) {
                        case NUMBER:
                            ret->value = malloc(sizeof(number));
                            *(number *)ret->value = *(number *)res->value;
                            break;
                        case STRING:
                            ret->value = strdup((char *) res->value);
                            break;
                        case FUNCTION:
                        case CFUNCTION:
                            ret->value = res->value;
                            break;
                        case OBJECT:
                            // objects are just referenced
                            ret->value = res->value;
                            break;
                    }

                }

                break;
        }

        // evaluate code into object, except requested
        if(flags != RAW && ret->type == CODE) {

            // create new opargs for virtual environment
            opargs new_args;

            // execute the CODE's value
            new_args.code = ret->value;

            // create a new hashtable
            new_args.hashtable = create_hashtable(8, 4);
            // set parent table
            new_args.hashtable->parent = args->hashtable;

            // copy debug info
            new_args.info = args->info;

            // iterate over each functional instruction, as long as the stack is not empty
            while(new_args.code->current != new_args.code->start) {
                _recursiveprocess(&new_args, REFERENCE);
            }

            // change type
            ret->type = OBJECT;

            // return the object's table
            ret->value = new_args.hashtable;

            // free the code copy
            free(new_args.code);
        }

        return ret;
    }
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
    mementry *dst = _recursiveprocess(args, REFERENCE); // Load only operand into mementry

    if(dst->type == NUMBER) {
        (*(number*)dst->value)++;
    } else
        throw(EI_INVALID_COMBINATION, args->info);
    return dst;
}

mementry *_decr(opargs *args) {
    mementry *dst = _recursiveprocess(args, REFERENCE); // Load only operand into mementry

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
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value *= *(number *)tmp->value;
                break;
            }
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_div(opargs *args) {
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value /= *(number *)tmp->value;
                break;
            }
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
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value += *(number *)tmp->value;
                break;
            }
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_sub(opargs *args){
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value -= *(number *)tmp->value;
                break;
            }
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_let(opargs *args){
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value *= *(number *)tmp->value;
                break;
            }
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_leq(opargs *args){
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value = *(number *)dst->value  <= *(number *)dst->value;
                break;
            }
        default:
            throw(EI_INVALID_COMBINATION, args->info);
            break;
    }
    free_mementry(tmp);

    return dst;
}

mementry *_grt(opargs *args){
    mementry *tmp = _recursiveprocess(args, COPY); // Load right operand into tmp mementry
    mementry *dst = _recursiveprocess(args, COPY); // Load left operand into dst mementry

    switch (dst->type) {
        case NUMBER:
            if(tmp->type == NUMBER){
                *(number *)dst->value = *(number *)dst->value > *(number *)dst->value;
                break;
            }
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
    // copy defaults, in case they're used by different functions
    mementry *code = _recursiveprocess(args, RAW);
    mementry *defaults = _recursiveprocess(args, COPY);

    // TODO: assert defaults is OBJECT and code is CODE

    // create the function


}

mementry *_ass(opargs *args){
    mementry *src = _recursiveprocess(args, COPY); // Load right operand into src
    mementry *dst = _recursiveprocess(args, REFERENCE); // Load left operand into dst

    // copy type
    dst->type = src->type;

    // free original dst value
    // ? free(dst->value);

    // create value reference
    dst->value = src->value;

    // free src
    // ? free(src);

    return dst;
}

mementry *_hardset(opargs *args) {
    mementry *src = _recursiveprocess(args, COPY); // Load right operand into src
    mementry *dst = _recursiveprocess(args, DIRECT); // Load left operand into dst

    // copy type
    dst->type = src->type;

    // free original dst value
    // ? free(dst->value);

    // create value reference
    dst->value = src->value;

    // free src
    // ? free(src);

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

mementry *_list(opargs *args){
    mementry *right = _recursiveprocess(args, REFERENCE);
    mementry *left = _recursiveprocess(args, REFERENCE);

    if(left->type == TUPLE) {
        if(right->type == TUPLE) {
            // add values of right to left
            set(left->value, right->value, ((array *)left->value)->size);
        } else {
            // copy right without expanding it as a tuple
            set_element(left->value, right, ((array *)left->value)->size);
        }
        return left;
    } else if (right->type == TUPLE){
        set_element(right->value, left, 0);
        return right;
    } else {
        // create new array
        array *arr = create_array(DEFAULT_ARRAY_SIZE);

        // set elements
        set_element(arr, left, 0);
        set_element(arr, right, 1);

        // create new mementry for array
        mementry *ret = malloc(sizeof(mementry));
        ret->type = TUPLE;
        ret->value = arr;
        return ret;
    }
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
    mementry *params = _recursiveprocess(args, COPY); // Load function arguments
    mementry *dst = _recursiveprocess(args, COPY); // Load function

    switch(dst->type) {

        case FUNCTION:
            // clone stack (for thread safety)
            bytecode clone = *(bytecode *)dst->value;

            // create new opargs for virtual environment
            opargs new_args;
            new_args.code = &clone;

            // create a new, small hashtable for the function to be isolated (without parent table)
            new_args.hashtable = create_hashtable(8, 4);

            // TODO: put arguments into table

            // copy debug info
            new_args.info = args->info;

            // the default return value
            dst = NULL;

            // iterate over each functional instruction, as long as the stack is not empty
            while(clone.current != clone.start) {
                dst = _recursiveprocess(&new_args, REFERENCE);
            }

            // ree table
            free_hashtable(new_args.hashtable);

            // free arguments
            free(params);

            // call the function and return the result
            return dst;

        case CFUNCTION:
            // call function from shared object
            callfunc((cfunction *) dst->value, params, dst);

            // return the result that's stored in dst now
            return dst;

        default:
            // Throw an error if its not a function
            throw(EI_NOT_CALLABLE, args->info);
            return dst;
    }
}

mementry *_index(opargs *args){
}

mementry *_array(opargs *args){
    // array of references
    mementry *content = _recursiveprocess(args, REFERENCE);

    // if tuple, just make array
    if(content->type == TUPLE){
        content->type = ARRAY;
        return content;
    }

    // create new array
    array *arr = create_array(DEFAULT_ARRAY_SIZE);

    // set elements
    set_element(arr, content, 0);

    // create new mementry for array
    mementry *ret = malloc(sizeof(mementry));
    ret->type = ARRAY;
    ret->value = arr;
    return ret;
}

mementry *_block(opargs *args){
    // move ht scope up
    ht_up(args->hashtable);
    return NULL;
}

mementry *_block_end(opargs *args){
    // move ht scope down
    ht_down(args->hashtable);
    return NULL;
}
