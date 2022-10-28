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


// free a synthetic mementry and its synthetic value
void _free_synth(mementry *entry) {
    if(entry->flags.synthetic) {
        if(entry->flags.value_synthetic && entry->type != UNDEFINED)
            free(entry->value);
        free(entry);
    }
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
        memptr->flags = (struct s_mementry_flags) {.mutable = 0, .synthetic = 1, .value_synthetic = 0};
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
            if(memptr->flags.mutable)
                return memptr;
        }
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
    mementry *dst = _recursiveprocess(args, MUTABLE); // Load only operand into mementry
    if(dst == NULL){
        throw(EI_REQ_MUTABLE, args->info);
        return NULL;
    }

    if(dst->type != NUMBER){
       throw(EI_WRONG_TYPE, args->info);
       return NULL;
    }

    (*(number*)dst->value) = !(*(number*)dst->value);
    return dst;
}

mementry *_bnot(opargs *args) {
    mementry *dst = _recursiveprocess(args, MUTABLE); // Load only operand into mementry
    if(dst == NULL){
        throw(EI_REQ_MUTABLE, args->info);
        return NULL;
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
    if(lo->flags.synthetic)\
        dst = lo;\
    else if(ro->flags.synthetic)\
        dst = ro;\
    else {\
        dst = _alloc_mementry();\
        dst->value = malloc(sizeof(number));\
        dst->type = NUMBER;\
        dst->flags = \
        (struct s_mementry_flags) {.mutable=0, .synthetic=1, .value_synthetic=1};\
    }\
    \
    number *valdst = dst->flags.value_synthetic ? dst->value : malloc(sizeof(number));\
    \
    /* perform the requested calculation */\
    *valdst = calculation;\
    \
    if(!dst->flags.value_synthetic){\
        dst->flags.value_synthetic = 1;\
        dst->value = valdst;\
    }\
    \
    /* free right operand if it's not the entry to be returned */\
    if(dst != ro)\
        _free_synth(ro);\
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
    if(lo->flags.synthetic)
        dst = lo;
    else if(ro->flags.synthetic)
        dst = ro;
    else {
        dst = _alloc_mementry();
        dst->value = malloc(sizeof(number));
        dst->type = ro->type;
        dst->flags = 
        (struct s_mementry_flags) {.mutable=0, .synthetic=1, .value_synthetic=1};
    }
    
    char *valdst = dst->flags.value_synthetic ? dst->value : 
        malloc(ro->type == STRING ? strlen((char *)ro->value)+strlen((char *)lo->value) + 1 : sizeof(number));
    
    /* perform the requested calculation */
    if(ro->type == STRING){
        strcpy(valdst, (char *) lo->value);
        strcat(valdst, (char *) ro->value);
    } else
        *(number *)valdst = a + b;
    
    if(!dst->flags.value_synthetic){
        dst->flags.value_synthetic = 1;
        dst->value = valdst;
    }
    
    /* free right operand if it's not the entry to be returned */
    if(dst != ro)
        _free_synth(ro);
    
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
    if(lo->flags.synthetic)
        dst = lo;
    else if(ro->flags.synthetic)
        dst = ro;
    else {
        dst = _alloc_mementry();
        dst->value = malloc(sizeof(number));
        dst->type = NUMBER;
        dst->flags = 
            (struct s_mementry_flags) {.mutable=0, .synthetic=1, .value_synthetic=1};
    }
    
    number *valdst = dst->flags.value_synthetic ? dst->value : malloc(sizeof(number));
    
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
    
    if(!dst->flags.value_synthetic){
        dst->flags.value_synthetic = 1;
        dst->value = valdst;
    }
    
    /* free right operand if it's not the entry to be returned */
    if(dst != ro)
        _free_synth(ro);
    
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

mementry *_lambda(opargs *args){

}

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
    // ? free(dst->value);

    // create value reference
    dst->value = src->value;

    // reset flags
    dst->flags = 
        (struct s_mementry_flags) {.mutable = 0, .synthetic = 0, .value_synthetic = 0};

    // free src if synthetic
    if(src->flags.synthetic)
        free(src);

    return dst;
}

mementry *_hardset(opargs *args) {
    
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
        mementry *ret = _alloc_mementry();
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
    // if number, return itself, else return truth value
    mementry *o = _recursiveprocess(args, DEREFERENCE);

    if(o->type != NUMBER) {
        throw(EI_WRONG_TYPE, args->info);
        return NULL;
    }
    
    /* determine the dst entry */
    mementry *dst = NULL;
    if(o->flags.synthetic)
        dst = o;
    else {
        dst = _alloc_mementry();
        dst->value = malloc(sizeof(number));
        dst->type = NUMBER;
        dst->flags = 
            (struct s_mementry_flags) {.mutable=0, .synthetic=1, .value_synthetic=1};
    }
    
    if(dst != o) {
        *(number *)dst->value = *(number *)o->value;
    }

    if(!dst->flags.value_synthetic)
        dst->flags.value_synthetic = 1;
    
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
                _g_params->flags = 
                    (struct s_mementry_flags) {.mutable = 0, .synthetic = 1, .value_synthetic = 0};

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
                    _free_synth(dst);
                dst = _recursiveprocess(&new_args, 0);
            }

            // reset arguments to parent function
            // ? _free_synth(_g_params);
            _g_params = reset;

            break;
        }

        case CFUNCTION:
            if(params->flags.synthetic)
                dst = params;
            else if(fun->flags.synthetic)
                dst = fun;
            else {
                // type and value will be set by the called function
                dst = _alloc_mementry();
            }

            // call function from shared object
            callfunc((cfunction *) fun->value, params, dst, info);

            // return the result that's stored in dst now
            break;

        default:
            // Throw an error if its not a function
            throw(EI_NOT_CALLABLE, info);
            _free_synth(fun);
            return NULL;
    }


    // free arguments
    if(dst != fun)
        _free_synth(fun);

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
            if(val->flags.value_synthetic)
                dst = val;
            else
                (dst = _alloc_mementry())->type = STRING;

            // create new substring
            char *substring = malloc(2);

            // load substring
            substring[0] = *((char *)val->value + i);
            substring[1] = '\0';

            // allocate previous string
            if(dst == val){
                if(val->flags.value_synthetic)
                    free(val->value);
            } else
                _free_synth(dst);

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
