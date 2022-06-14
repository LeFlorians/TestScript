#include <stdlib.h>
#include "../interpreter/tokenizer.h"

// n element of a hash table
typedef struct hashelement hashelement;

struct hashelement {
    // type (one of runtime/shared types)
    typing type;

    char *key;

    // a pointer to the value of the element
    // value should not contain the key, as that would not make sense memory-wise
    void *value;

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

// Function to return pointer to address of value given by key (never NULL)
hashelement *find(hashtable *table, char* key);