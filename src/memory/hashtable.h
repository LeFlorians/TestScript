#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "../interpreter/tokenizer.h"

typedef struct {
    // type (one of runtime/shared types)
    typing type;

    /*
        Value pointer type depends on type
            NUMBER              number *
            FIELD               char *
            STRING              char *

            ARRAY, TUPLE        array *
            OBJECT              hashtable *
            CODE                bytecode *
            FUNCTION, CFUNCTION function *
    */
    void *value;

} mementry;

// internally used hash types
typedef uint16_t _H_HASH;
#define _H_MAX_VALUE UINT16_MAX

// TODO: compare performance
// disable interpolation search as it is slower due to per-step overhead
#define _H_INTERPOLATION_SEARCH 0

// elements in the slices
typedef struct s_tableentry tableentry;
typedef struct s_tableentry {
    // key in object table
    char *key;
    // the key's hash
    _H_HASH hash;

    // alternative entry, in case two share the same hash
    tableentry *alternative;

    // associated data value (linked-list approach)
    mementry *entry;
} tableentry;

// slices in the object table (acts as dynamic-length sorted list)
typedef struct {
    size_t size_allocated, size;

    tableentry *array[];
} tableslice;


// object table sturct
typedef struct {

    // width of the hash-table (fixed)
    size_t width;

    tableslice *entries[];

} hashtable;

// create a table
hashtable *create_hashtable(size_t width);

// find mementry in object table
mementry *find(hashtable *table, char *key, char allocate);

// free a mementry and its value
void free_mementry(mementry *entry);