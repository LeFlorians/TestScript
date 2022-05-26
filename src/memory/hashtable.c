#include "hashtable.h"

// TODO: add cache

// create hash table
// @param size initial table size
hashtable *create_hashtable(size_t size){
    hashtable *ret = (hashtable *) malloc(sizeof(hashtable));

    ret->size = size;
    ret->array = (hashelement *) malloc(sizeof(hashelement) * size);

    // set default keys    
    for(hashelement *cur = ret->array; cur < ret->array + size; cur++)
        cur->key = NULL;
}

// find an element in the hash table
// returns NULL if not found
void *find(void *key, size_t keysize){

}

size_t hash(void *key, size_t keysize) {
    
}
