#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "../tokenizer.h"

// define datatype for level
typedef unsigned int table_level;

// define hashtable type
typedef struct s_hashtable hashtable;
typedef struct s_tableentry tableentry;

typedef struct {
    // type (one of runtime/shared types)
    typing type;

    // the hashmap-level this variable was created on (or zero for contants)
    table_level level;

    /*
        Value pointer type depends on type
            NUMBER              number *
            STRING              char *

            REFERENCE           mementry *

            EXPR                operation

            ARRAY, TUPLE        array *
            OBJECT              hashtable *
            CODE                bytecode *
            FUNCTION, CFUNCTION function *

            UNDEFINED           NULL
    */
    void *value;

    // some flags
    struct s_mementry_flags {
        // set if this mementry is a mutable field (mutable entry in hashtable)
        unsigned char mutable: 1;

        // > used by implementations.c < to see if
        // a mementry only exists during expression-evaluation
        unsigned char synthetic: 1; 
        // used to check if the value this mementry is pointing to is synthetic
        // this will only be set, if synthetic is set as well
        unsigned char value_synthetic: 1;
    } flags;

} mementry;

// function to allocate mementry
mementry *_alloc_mementry();

// internally used hash types
typedef uint32_t _H_HASH;
#define _H_MAX_VALUE UINT32_MAX

// TODO: compare performance
// disable interpolation search as it is slower due to per-step overhead
#define _H_INTERPOLATION_SEARCH 0

// elements in the slices
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
struct s_hashtable {

    // width of the hash-table (fixed)
    size_t width, cache_size;

    // the level of the table (modified by ht_up, ht_down)
    table_level level;

    // scope stack
    stack *stack;

    // stack offset
    size_t offset;

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
mementry *find(hashtable *table, char *key);

// free a mementry and its value
void free_mementry(mementry *entry);

// function to iterate over all table entries
void walk_table(hashtable *table, void (*callback)(tableentry *));
