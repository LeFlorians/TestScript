#include <math.h>

#include "implementations.h"
#include "../memory/array.h"
#include "../libraries/libraries.h" 

// ----- Helper functions -----

// flags for the processing function
#define FLAG_SET(flag) ((flag & flags) != 0)
// whether fields should be auto de-referenced
#define MUTABLE 1 // require the value of a reference to a mutable field
#define DEREFERENCE 2

#define DEFAULT_ARRAY_SIZE 8

// the right modulo function for the number type
#define MOD(a, b) (number) fmodl(a, b)

// return the truth-value of a mementry
char truth_of(mementry *entry) {
    if(entry == NULL)
        return 0;
    if(entry->type == REFERENCE)
        entry = entry->value;
    if(entry->type == UNDEFINED ||
            (entry->type == NUMBER && (*(number *)entry->value) == 0) ||
            (entry->type == STRING && (*(char *)entry->value) == '\0'))
        return 0;
    return 1;
}

mementry *_recursiveprocess(opargs *args, char flags) {
    // get the next mementry from the bytecode
    mementry *memptr = *(mementry **)pop(args->code, &args->offset, sizeof(mementry *));

    repeat:

    // check if NULL
    if(memptr == NULL){
        // return undefined instead 
        memptr = _alloc_mementry();
        memptr->type = UNDEFINED;
        memptr->value = NULL;
        return memptr;
    }

    // TODO: handle other cases
    // ! remember, the bytecode should not be changed unintentionally
    switch (memptr->type) {
        case EXPR:
            // execute subexpression and repeat
            memptr = ((operation)memptr->value)(args);

            // repeat this with the result in memptr
            goto repeat;
    }

    // require mutable field
    if(FLAG_SET(MUTABLE)) {
        // only return the value, if it's a mutable field
        if(memptr->type == REFERENCE) {
            memptr = memptr->value;
            if(memptr->flags.from_ht || memptr->flags.persistent)
                return memptr;
        }
        _free_synth(memptr, 1);
        // only return null in this case
        return NULL;
    }

    // dereference if field
    if(FLAG_SET(DEREFERENCE)) {
        if(memptr->type == REFERENCE)
            memptr = memptr->value;
    }

    return memptr;
}


// ----- Operator implementations -----


mementry *_incr(opargs *args) {
    mementry *dst = _recursiveprocess(args, MUTABLE); // Load only operand into mementry
    if(dst == NULL){
        throw(EI_REQ_MUTABLE, args->info);
        return NULL;
    }

    // do not free it, since it's certainly not synthetic
    if(dst->type != NUMBER){
       throw(EI_WRONG_TYPE, args->info);
       return NULL;
    }

    (*(number*)dst->value)++;
    return dst;
}

mementry *_decr(opargs *args) {
    mementry *dst = _recursiveprocess(args, MUTABLE); // Load only operand into mementry
    if(dst == NULL){
        throw(EI_REQ_MUTABLE, args->info);
        return NULL;
    }

    if(dst->type != NUMBER){
       throw(EI_WRONG_TYPE, args->info);
       return NULL;
    }

    (*(number*)dst->value)--;
    return dst;
}

mementry *_lnot(opargs *args) {
    mementry *src = _recursiveprocess(args, DEREFERENCE); // Load only operand into mementry
    mementry *dst;
    if(src->flags.from_ht || src->flags.persistent){
        dst = _alloc_mementry();
        dst->flags.val_tmp = 1;
        dst->value = malloc(sizeof(number));
    }

    if(dst->type != NUMBER){
       throw(EI_WRONG_TYPE, args->info);
       return NULL;
    }

    (*(number*)dst->value) = !(*(number*)dst->value);
    return dst;
}

mementry *_bnot(opargs *args) {
    mementry *src = _recursiveprocess(args, DEREFERENCE); // Load only operand into mementry
    mementry *dst;
    if(src->flags.from_ht || src->flags.persistent){
        dst = _alloc_mementry();
        dst->flags.val_tmp = 1;
        dst->value = malloc(sizeof(number));
    }

    if(dst->type != NUMBER){
       throw(EI_WRONG_TYPE, args->info);
       return NULL;
    }

    // ? maybe round the number
    (*(number*)dst->value) = -(*(number*)dst->value) - 1;
    return dst;
}

// macro for the following operations (in terms of a,b)
#define a (*(number *)lo->value)
#define b (*(number *)ro->value)
#define CALCULATE(calculation)\
    mementry *ro = _recursiveprocess(args, DEREFERENCE); /* Load right into tmp mementry */\
    mementry *lo = _recursiveprocess(args, DEREFERENCE); /* Load left into dst mementry */\
    \
    /* make sure only to calculate numbers */\
    if(ro->type != NUMBER || lo->type != NUMBER) {\
        throw(EI_WRONG_TYPE, args->info);\
        return NULL; \
    }\
    \
    /* determine the dst entry (or allocate a new one if ro and lo are not synthetics) */\
    mementry *dst = NULL;\
    if(lo->flags.val_tmp)\
        dst = lo;\
    else if(ro->flags.val_tmp)\
        dst = ro;\
    else {\
        dst = _alloc_mementry();\
        dst->type = NUMBER;\
    }\
    \
    number *valdst = dst->flags.val_tmp ? dst->value : malloc(sizeof(number));\
    dst->value = valdst; \
    dst->flags.val_tmp = 1; \
    \
    /* perform the requested calculation */\
    *valdst = calculation;\
    \
    /* free right operand if it's not the entry to be returned */\
    if(dst != lo && ro != lo && !lo->flags.from_ht)\
        _free_synth(lo, lo != dst);\
    if(dst != ro)\
        _free_synth(ro, ro != dst);\
    \
    return dst;


mementry *_mul(opargs *args){ CALCULATE(a * b) }

mementry *_div(opargs *args){ CALCULATE(a / b) }

mementry *_mod(opargs *args){ CALCULATE( MOD(a, b) ) }

mementry *_add(opargs *args){
    mementry *ro = _recursiveprocess(args, DEREFERENCE); /* Load right into tmp mementry */
    mementry *lo = _recursiveprocess(args, DEREFERENCE); /* Load left into dst mementry */
    
    /* make sure only to add numbers or strings (concat) */
    if((ro->type != NUMBER && ro->type != STRING) || lo->type != ro->type) {
        throw(EI_WRONG_TYPE, args->info);
        return NULL; 
    }
    
    /* determine the dst entry (or allocate a new one if ro and lo are not synthetics) */
    mementry *dst = NULL;
    if(lo->flags.val_tmp)
        dst = lo;
    else if(ro->flags.val_tmp)
        dst = ro;
    else {
        dst = _alloc_mementry();
        dst->type = ro->type;
    }
    
    void *valdst = dst->flags.val_tmp ? dst->value : 
        malloc(ro->type == STRING ? strlen((char *)ro->value)+strlen((char *)lo->value) + 1 : sizeof(number));
    dst->flags.val_tmp = 1;
    dst->value = valdst;
    
    /* perform the requested calculation */
    if(ro->type == STRING){
        strcpy(valdst, (char *) lo->value);
        strcat(valdst, (char *) ro->value);
    } else
        *(number *)valdst = a + b;
       
    /* free right operand if it's not the entry to be returned */
    if(dst != lo && ro != lo)
        _free_synth(lo, lo != dst);
    if(dst != ro)
        _free_synth(ro, lo != dst);
    
    return dst;

}

mementry *_sub(opargs *args){ CALCULATE(a - b) }

mementry *_let(opargs *args){ CALCULATE(a < b) }

mementry *_leq(opargs *args){ CALCULATE(a <= b) }

mementry *_grt(opargs *args){ CALCULATE(a > b) }

mementry *_geq(opargs *args){ CALCULATE(a >= b) }

mementry *_equ(opargs *args){
    mementry *ro = _recursiveprocess(args, DEREFERENCE); /* Load right into tmp mementry */
    mementry *lo = _recursiveprocess(args, DEREFERENCE); /* Load left into dst mementry */
    
    /* determine the dst entry (or allocate a new one if ro and lo are not synthetics) */
    mementry *dst = NULL;
    if(lo->flags.val_tmp)
        dst = lo;
    else if(ro->flags.val_tmp)
        dst = ro;
    else {
        dst = _alloc_mementry();
        dst->type = NUMBER;
    }
    
    number *valdst = dst->flags.val_tmp ? dst->value : malloc(sizeof(number));
    dst->flags.val_tmp = 1;
    dst->value = valdst;
    
    // check if equal
    // for stings, numbers, this checks contents
    // for other types, checks memory location
    if(lo->type != ro->type)
        *valdst = 0;
    else {
        if(lo->type == STRING) {
            *valdst = (strcmp((char *)lo->value, (char *)ro->value) == 0);
        } else if(lo->type == NUMBER) {
            *valdst = (a == b); // a,b are numeric values of ro,lo
        } else {
            *valdst = (lo->value == ro->value);
        }
    }
       
    /* free right operand if it's not the entry to be returned */
    if(dst != lo && !lo->flags.from_ht)
        _free_synth(lo, lo != dst);
    if(dst != ro)
        _free_synth(ro, lo != dst);
    
    return dst;
}

mementry *_nequ(opargs *args){
    // just perform equ and negate result for simplicity
    mementry *res = _equ(args);
    (*(number *)res->value) = !(*(number *)res->value);
    return res;
}

mementry *_band(opargs *args){ 
    CALCULATE( (number) ((long long)a & (long long) b) ) }

mementry *_bxor(opargs *args){
    CALCULATE( (number) ((long long)a ^ (long long) b) ) }

mementry *_bor(opargs *args){
    CALCULATE( (number) ((long long)a | (long long) b) ) }

mementry *_land(opargs *args){ CALCULATE(a && b) }

mementry *_lor(opargs *args){ CALCULATE(a || b) }

mementry *_ass(opargs *args){
    mementry *src = _recursiveprocess(args, 0); // Load right operand into src
    mementry *dst = _recursiveprocess(args, 0); // Load left operand into dst

    if(dst->type != REFERENCE) {
        throw(EI_REQ_MUTABLE, args->info);
        return NULL;
    }

    if(src->type == REFERENCE || src->type == FUNCTION) {
        // make sure that dst exists higher than source
        if(src->value != NULL && dst->level < src->level) {
            throw(EI_UNLEVELED, args->info);
            return NULL;
        }

    }

    if(src->type == REFERENCE)
        // dereference field
        src = src->value;

    // dereference field
    dst = dst->value;

    // copy type
    dst->type = src->type;

    // free original dst value
    _free_synth(dst, 0);

    // create value reference
    dst->value = src->value;

    // reset flags
    dst->flags.val_tmp = 0;

    // free src
    if(!src->flags.from_ht && !src->flags.persistent)
        free(src);

    return dst;
}

mementry *_list(opargs *args){
    mementry *right = _recursiveprocess(args, DEREFERENCE);
    mementry *left = _recursiveprocess(args, DEREFERENCE);

    if(left->type == TUPLE) {
        if(right->type == TUPLE) {
            // add values of right to left
            set(left->value, right->value, ((array *)left->value)->size);
        } else {
            // copy right without expanding it as a tuple
            set_element(left->value, right, ((array *)left->value)->size);
        }

        // free right tuple
        _free_synth(right, 1);

        return left;
    } else if (right->type == TUPLE){
        set_element(right->value, left, 0);
        _free_synth(left, 1);
        return right;
    } else {
        // create new array
        array *arr = create_array(DEFAULT_ARRAY_SIZE);

        // set elements
        set_element(arr, left, 0);
        set_element(arr, right, 1);

        // don't free left/right here, they are in arr

        // create new mementry for array
        mementry *ret = _alloc_mementry();
        ret->type = TUPLE;
        ret->value = arr;
        return ret;
    }
}

mementry *_pos(opargs *args){
    // if number, return itself, else return truth value
    mementry *o = _recursiveprocess(args, DEREFERENCE);

    if(o->type != NUMBER) {
        throw(EI_WRONG_TYPE, args->info);
        return NULL;
    }
    
    /* determine the dst entry */
    mementry *dst = NULL;
    if(o->flags.val_tmp)
        dst = o;
    else {
        dst = _alloc_mementry();
        dst->value = malloc(sizeof(number));
        dst->type = NUMBER;
        dst->flags.val_tmp = 1;
        *(number *)dst->value = *(number *)o->value;
    }

    if(!o->flags.from_ht && !o->flags.persistent)
        free(o);
   
    return dst;
}

mementry *_neg(opargs *args){
    // negate the output of _pos for simplicity
    mementry *res = _pos(args);
    *(number *)res->value = -(*(number *)res->value);
    return res;
}


// global args object used for function calls
mementry *_g_params = NULL;

mementry *_args(opargs *args) {
    // used in functions to retreive current function arguments
    return _g_params;
}

mementry *call(mementry *fun, mementry *params, errorinfo *info){
    // mementry for the result
    mementry *dst = NULL;

    switch(fun->type) {

        case FUNCTION: {

            // create new opargs for virtual environment
            opargs new_args;

            // reference the function code stack
            new_args.code = (bytecode *)fun->value;

            // register arguments, assert tuple 
            mementry *reset = _g_params;
            if(params->type != TUPLE) {
                array *arr = create_array(1);

                _g_params = _alloc_mementry();
                _g_params->type = TUPLE;
                _g_params->value = arr;
                _g_params->flags.val_tmp = 1;

                set_element(arr, params, 0);
            } else
                _g_params = params;

            // copy debug info
            new_args.info = info;

            // create a stack pointer, starting at the end
            new_args.offset = new_args.code->elements;

            // iterate over each functional instruction, as long as stack not empty
            while(new_args.offset != 0) {
                if(dst != NULL)
                    _free_synth(dst, 1);
                dst = _recursiveprocess(&new_args, 0);
            }

            // reset arguments to parent function
            _free_synth(_g_params, 1);
            _g_params = reset;

            break;
        }

        case CFUNCTION:
            if(params->flags.val_tmp)
                dst = params;
            else if(fun->flags.val_tmp)
                dst = fun;
            else {
                // type and value will be set by the called function
                dst = _alloc_mementry();
                dst->flags.val_tmp=1;
            }

            // call function from shared object
            callfunc((cfunction *) fun->value, params, dst, info);

            // return the result that's stored in dst now
            break;

        default:
            // Throw an error if its not a function
            throw(EI_NOT_CALLABLE, info);
            _free_synth(fun, 1);
            return NULL;
    }


    // free arguments
    if(dst != fun)
        _free_synth(fun, 1);

    return dst;
}

mementry *_call(opargs *args){
    mementry *params = _recursiveprocess(args, DEREFERENCE); // Load arguments
    mementry *fun = _recursiveprocess(args, DEREFERENCE); // Load function

    return call(fun, params, args->info);
}

mementry *_index(opargs *args){
    mementry *index = _recursiveprocess(args, DEREFERENCE);
    mementry *val = _recursiveprocess(args, DEREFERENCE);
  
    if(index->type == NUMBER) {
        size_t i = (size_t) *(number *)index->value;

        if(val->type == STRING) {
            if(i < 0 || i >= strlen((char *)val->value)) {
                throw(EI_INDEX_OOB, args->info);
                return NULL;
            }
           
            // create destination register
            mementry *dst;
            if(val->flags.val_tmp)
                dst = val;
            else {
                dst = _alloc_mementry();
                dst->type = STRING;
                dst->flags.val_tmp = 1;
            }

            // create new substring
            char *substring = malloc(2);

            // load substring
            substring[0] = *((char *)val->value + i);
            substring[1] = '\0';

            // free val if synthetic
            _free_synth(val, val != dst);

            // set and return substring
            dst->value = substring;
            return dst; 
        } else {
            if(i < 0 || i >= ((array *)val->value)->size) {
                throw(EI_INDEX_OOB, args->info);
                return NULL;
            }
            mementry * res = get((array *)val->value, i);
            return res;
        }
    }
    throw(EI_WRONG_TYPE, args->info);
    return NULL;
 }

mementry *_array(opargs *args){
    // array of references
    mementry *content = _recursiveprocess(args, DEREFERENCE);

    // if tuple, just make array
    if(content->type == TUPLE){
        content->type = ARRAY;
        return content;
    }

    // create new array
    array *arr = create_array(DEFAULT_ARRAY_SIZE);

    // set elements
    set_element(arr, content, 0);

    // don't free content because it's in array

    // create new mementry for array
    mementry *ret = _alloc_mementry();
    ret->type = ARRAY;
    ret->value = arr;
    return ret;
}

mementry *_block(opargs *args){
    return NULL;
}

mementry *_block_end(opargs *args){
    return NULL;
}
