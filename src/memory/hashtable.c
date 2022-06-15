#include <string.h>

#include "hashtable.h"


// TODO: implement
// TODO: add cache

// create hash table
// @param width initial table width
hashtable *create_hashtable(size_t width){
    hashtable *ret = (hashtable *) malloc(sizeof(hashtable));

    ret->width = width;
    ret->array = (hashelement *) malloc(sizeof(hashelement) * width);

    // set default keys, alternatives
    for(hashelement *cur = ret->array; cur < ret->array + width; cur++) {
        cur->key = NULL;
        cur->alternative = NULL;
    }

    return ret;
}

// Define internally used hash type
typedef unsigned long long int hash;

// hash function
// TODO: optimize, ensure uniqueness
hash _hash(char* key) {
    char ch;
    hash ret = 0;
    while( (ch = *(key++)) != '\0')
        ret = (ret + ch) * ch;
    return ret;
}

// find an element in the hash table
hashelement *find(hashtable *table, char* key) {
    size_t pos = (_hash(key) % table->width);

    // go through elements at that index ( avg. O(n/width * strsize) )
    hashelement *first = &table->array[pos];
    hashelement *current = first;
    while(current->key == NULL || strcmp(current->key, key) != 0) {
        if(current->key == NULL) {
            // not found, allocate a new element to append
            hashelement *new = malloc(sizeof(hashelement));
            new->key = NULL;
            new->alternative = NULL;

            // append new element
            current->alternative = new;

            // modify the value to return
            current->key = key;

            // set default type and value
            current->type = EMPTY;
            current->value = NULL;

            break;
        }

        current = current->alternative;
    }

    return current;
}