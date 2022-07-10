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
typedef uint32_t _H_HASH;
#define _H_MAX_VALUE UINT32_MAX

// TODO: compare performance
// disable interpolation search as it is slower due to per-step overhead
#define _H_INTERPOLATION_SEARCH 0

// elements in the slices
typedef struct s_tableentry tableentry;
struct s_tableentry {
    // key in object table
    char *key;
    // the key's hash
    _H_HASH hash;

    // alternative entry, in case two share the same hash (linked-list approach)
    tableentry *alternative;

    // associated data value
    mementry *entry;
};

// slices in the object table (acts as dynamic-length sorted list)
typedef struct {
    size_t size_allocated, size;

    tableentry *array[];
} tableslice;


// object table sturct
typedef struct s_hashtable hashtable;
struct s_hashtable {

    // width of the hash-table (fixed)
    size_t width, cache_size;

    // scope stack
    stack *stack;

    // the parent table of objects (NULL for root table)
    hashtable *parent;

    // direct mapped cache for faster lookups
    tableentry **cache;

    // hashtables slices (buckets)
    tableslice *entries[];

};

// create/free a table
hashtable *create_hashtable(size_t width, size_t cache_size);
void free_hashtable(hashtable *table);

// move up/down scopes
void ht_up(hashtable *table);
void ht_down(hashtable *table);

// find mementry in object table
mementry *find(hashtable *table, char *key, char allocate);

// free a mementry and its value
void free_mementry(mementry *entry);

// function to iterate over all table entries
void walk_table(hashtable *table, void (*callback)(tableentry *));