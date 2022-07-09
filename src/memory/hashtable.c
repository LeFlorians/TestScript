#include <string.h>

#include "hashtable.h"
#include "../interpreter/bytecode.h"

#define DEFAULT_ENTRY_SIZE 8

// hash function
// TODO: optimize, ensure uniqueness, EQUAL DISTRIBUTION !!
_H_HASH _hash(char* key) {
    char ch;
    _H_HASH ret = 0;
    while((ch = *(key++)) != '\0')
        ret = (ret + ch) * ch;
    return ret;
}

hashtable *create_hashtable(size_t width) {
    hashtable *ret = (hashtable *) malloc(sizeof(hashtable) + width * sizeof(tableslice *));

    ret->width = width;

    // initialize to NULL
    memset(ret->cache, 0, sizeof(tableentry *) * HASHTABLE_CACHE_SIZE);
    memset(ret->entries, 0, sizeof(tableslice *) * width);

    return ret;
}


mementry *find(hashtable *table, char *key, char allocate) {
    _H_HASH hash = _hash(key);
    tableentry *value;

    // cache lookup
    value = table->cache[hash % HASHTABLE_CACHE_SIZE];

    // check if key matches
    if(value != NULL && strcmp(key, value->key) == 0) {
        return value->entry;
    }

    printf("hash not in cache for %s\n", key);

    // get the right table entry
    tableslice **sliceptr = table->entries + (hash % table->width);

    // allocate a new row if it does not yet exist
    if(*sliceptr == NULL){
        size_t size = sizeof(tableslice) + DEFAULT_ENTRY_SIZE * sizeof(tableentry *);

        // allocate new entry*
        *sliceptr = (tableslice *) malloc(size);

        if(*sliceptr == NULL) {
            // TODO: throw error, out of memory; slice could not be allocated
            return NULL;
        }

        // set default values
        memset(*sliceptr, 0, size);

        // set size allocated
        (*sliceptr)->size_allocated = DEFAULT_ENTRY_SIZE;
    }

    // dereference to tableslice *
    tableslice *slice = *sliceptr;

    // predict the index, assuming an equal distribution
    size_t top, bottom = 0;
    size_t index = ((top = slice->size) * hash) / (_H_MAX_VALUE + 1);

    #if _H_INTERPOLATION_SEARCH
        _H_HASH top_value = _H_MAX_VALUE, bottom_value = 0;
    #endif

    // binary search
    tableentry *new_value;
    while(top > bottom) {
        value = slice->array[index];

        // if hashes don't match, perform interpolation
        
        #if _H_INTERPOLATION_SEARCH

        if(value->hash < hash) {
            bottom = index + 1;
            bottom_value = value->hash;

            // the new range is now ]index, top[
            index = (top - index -1) * (hash - value->hash) / (top_value - value->hash) + index +1;
        } else if(value->hash > hash) {
            top = index;
            top_value = value->hash;

            // the new range is now [bottom, index[
            index = (index - bottom) * (hash - bottom_value) / (value->hash - bottom_value) + bottom;
        } else {

        
        #else

        if(value->hash < hash) {
            bottom = index + 1;

            index = (top + index) / 2;
        } else if(value->hash > hash) {
            top = index;

            index = (index + bottom) / 2;
        } else {

        #endif
            // matching hash!

            // compare keys
            printf("matching hash for %s (%s): %u == %u\n", key, value->key, hash, value->hash);
            while(strcmp(key, value->key)) {
                printf(".");
                // walk linked-list
                if(value->alternative == NULL) {
                    // not found, allocate or return NULL
                    if(!allocate)
                        return NULL;

                    // attach at end of chain
                    value->alternative = new_value = malloc(sizeof(tableentry));

                    // initialize entry
                    goto init_entry;
                }
                value = value->alternative;
            }

            // register entry in cache
            table->cache[hash % HASHTABLE_CACHE_SIZE] = value;

            // return the found data
            return value->entry;
        }

    }

    // hash not found
    value = slice->array[index];

    // get insertion index
    index = ((value != NULL && value->hash <= hash) ? (index + 1) : (index));

    // check if a new element fits
    if(slice->size + 1 > slice->size_allocated) {
        // allocate new slice
        tableslice *newslice = malloc(sizeof(tableslice) + slice->size_allocated * 2 * sizeof(tableentry *));
        newslice->size_allocated = slice->size_allocated * 2;
        newslice->size = slice->size + 1;

        if(newslice == NULL) {
            // TODO: out of memory error
            return NULL;
        }

        // move everything before index to where it should be
        memcpy(newslice->array, slice->array, index);

        // move everything after index one to the right
        memcpy(newslice->array + index + 1, slice->array + index, slice->size - index);

        // set new slice
        *sliceptr = newslice;

        // free old slice (not entries)
        free(slice);

        // insert at index into array
        newslice->array[index] = new_value = malloc(sizeof(tableentry));

        // init new element and return it
    } else {
        // move everything after index one to the right
        memcpy(slice->array + index + 1, slice->array + index, slice->size - index);

        // insert at index into array
        slice->array[index] = new_value = malloc(sizeof(tableentry));

        // increment slice size
        slice->size++;
    }

    init_entry:

    new_value->key = strdup(key);
    new_value->hash = hash;
    new_value->alternative = NULL;

    // allocate the wanted mementry and set default value
    (new_value->entry = malloc(sizeof(mementry)))->type = NUMBER;
    *((number *) (new_value->entry->value = malloc(sizeof(number)))) = (number)0;

    // register entry in cache
    table->cache[hash % HASHTABLE_CACHE_SIZE] = new_value;

    // return new entry
    return new_value->entry;

}

void free_mementry(mementry *entry) {
    
    // free the value
    free(entry->value);

    // free the entry itsself
    free(entry);
}