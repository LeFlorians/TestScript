#include <stdlib.h>

// n element of a hash table
typedef struct hashelement hashelement;

struct hashelement {
    // TODO: decide if void * or char * as key
    void *key;

    // a pointer to the value of the element
    // value should not contain the key, as that would not make sense memory-wise
    void *value;

    // for collision prevention
    hashelement *alternative;
};


// a hash table struct
typedef struct {
    // array of data pointers
    hashelement *array;

    // size of currently allcoated space
    size_t size;
} hashtable;