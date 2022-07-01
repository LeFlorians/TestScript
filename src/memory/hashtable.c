#include <string.h>

#include "hashtable.h"
#include "../interpreter/bytecode.h"

// TODO: add cache

// create hash table
// @param width initial table width
hashtable *create_hashtable(size_t width){
    hashtable *ret = (hashtable *) malloc(sizeof(hashtable));

    ret->width = width;
    ret->array = (hashelement **) malloc(sizeof(hashelement *) * width);

    // initialize to NULL
    memset(ret->array, 0, sizeof(hashelement *) * width);

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
// @param allocate if set and key was not found, automatically allocate it; return NULL if unset
mementry *find(hashtable *table, char allocate, char* key) {
    size_t pos = (_hash(key) % table->width);

    // go through elements at that index ( avg. O(n/width * strsize) )
    hashelement *start = table->array[pos];
    hashelement *dataptr = start;

    while(dataptr != NULL) {

        if(strcmp(dataptr->key, key) == 0) {
            // return the found element
            return dataptr->valueptr;
        }

        // key was not right, move on
        dataptr = dataptr->alternative;
    }

    // return NULL if the element should not be allcoated
    if(!allocate)
        return NULL;

    // allocate the missing hashelement
    hashelement *new = malloc(sizeof(hashelement));
    new->key = strdup(key);

    // set the alternative to be the current first element in the chain
    //  according to the locality of reference
    new->alternative = start;

    // make it the new array start
    table->array[pos] = new;

    
    // allocate default value mementry
    mementry *entry = malloc(sizeof(mementry));

    // set type
    entry->type = NUMBER;

    // allocate and set default number
    entry->value = malloc(sizeof(number));
    *(number *)entry->value = (number) 0;

    // assign to hashelement
    new->valueptr = entry;

    // return the allocated mementry
    return entry;
}

void free_mementry(mementry *entry) {
    
    // free the value
    free(entry->value);

    // free the entry itsself
    free(entry);
}