#include <stdlib.h>
#include "../interpreter/tokenizer.h"

// n element of a hash table
typedef struct hashelement hashelement;

typedef struct {
    // type (one of runtime/shared types)
    typing type;

    // can by any of (char *), (number *)
    void *value;

} mementry;

struct hashelement {
    char *key;

    // a pointer to the value of the element
    // value should not contain the key, as that would not make sense memory-wise
    mementry *valueptr;

    // for collision prevention
    hashelement *alternative;
};


// a hash table struct
typedef struct {
    // size of base array
    size_t width;

    // array of data pointers size 'width'
    hashelement *array;

} hashtable;

// function to create a hashtable
hashtable *create_hashtable(size_t width);

// Function to return pointer to address of value given by key (never NULL)
hashelement *find(hashtable *table, char allocate, char* key);

// Frees a mementry and the value that is contained within it
void free_mementry(mementry *entry);
