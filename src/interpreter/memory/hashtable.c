#include <string.h>

#include "hashtable.h"
#include "array.h"
#include "../bytecode.h"

#define DEFAULT_ENTRY_SIZE 8

// hash function
// TODO: optimize
static inline _H_HASH _hash(char* key) {
    _H_HASH hash = 0;
    char c;
    while((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

// return new mementry with default values
mementry *_alloc_mementry() {
    mementry *ret = malloc(sizeof(mementry));
    ret->value = NULL;
    ret->flags =
        (struct s_mementry_flags) 
        {.from_ht= 0,.val_tmp=0,.persistent=0};
    return ret;
}

// free a synthetic mementry and its synthetic value
void _free_synth(mementry *entry, char free_entry) {
    if((!entry->flags.from_ht || free_entry & IGNORE_HASHTABLE) 
            && (!entry->flags.persistent || free_entry & IGNORE_PERSISTENCE)) {
        if(entry->flags.val_tmp){
            switch(entry->type){
                case UNDEFINED:
                    break;
                case ARRAY:
                case TUPLE:
                    free_array((array *) entry->value);
                    break;
                case FUNCTION:
                    free_bytecode((bytecode *)entry->value, IGNORE_PERSISTENCE | IGNORE_HASHTABLE);
                    break;
                default:
                    free(entry->value);
                    break; 
            } 
    if(free_entry)
        free(entry);
        }
    }
}


hashtable *create_hashtable(size_t width, size_t cache_size) {
    hashtable *ret = (hashtable *) malloc(sizeof(hashtable) + width * sizeof(tableslice *));

    ret->width = width;
    ret->cache_size = cache_size;

    // allocate stack (with a guessed size)
    ret->stack = create_stack(width << 2);

    // set offset to 0
    ret->offset = 0;

    // make parent table NULL
    ret->parent = NULL;

    // set table level to 0 by default
    ret->level = 0;

    // allocate cache
    ret->cache = malloc(cache_size * sizeof(tableentry *));

    // initialize to NULL
    memset(ret->cache, 0, sizeof(tableentry *) * cache_size);
    memset(ret->entries, 0, sizeof(tableslice *) * width);

    return ret;
}

// private function to delete table entries
static inline void _free_tableentry(tableentry *entry) {
    free(entry->key);

    // TODO: free the mementries / data values
    // free(entry->entry->value);
    
    _free_synth(entry->entry, IGNORE_PERSISTENCE | IGNORE_HASHTABLE | 1);
    free(entry);
}

void free_hashtable(hashtable *table){
    // free stack
    free_stack(table->stack);

    // free cache
    free(table->cache);

    // free table slices
    tableslice *sliceptr;
    tableentry *entry, *next;
    for(size_t i = 0; i < table->width; i++){
        sliceptr = table->entries[i];

        // free all entries
        if(sliceptr != NULL)
            for(size_t j = 0; j < sliceptr->size; j++) {
                entry = sliceptr->array[j];
                // free entry and its alternatives
                while(entry != NULL){
                    next = entry->alternative;
                    _free_tableentry(entry);
                    entry = next;
                }
            }

    }

    // free table
    free(table);
}

void ht_up(hashtable *table) {
    // push NULL to the stack
    *((tableentry ***)push(table->stack, &table->offset, sizeof(tableentry **))) = NULL;
    // increase level
    // TODO: prevent overflow
    table->level++;
}

void ht_down(hashtable *table) {
    // free everything until NULL
    stack *s = table->stack;
    tableentry ***stackptr;
    tableentry **entryptr;
    tableentry *entry;

    // decrease level
    table->level--;

    while((stackptr = pop(s, &table->offset, 
                    sizeof(tableentry **))) != NULL && (entryptr = *stackptr) != NULL) {
        // copy the entry
        entry = *entryptr; // never null

        // remove it from cache if possible by replacing it with its alternative
        if(table->cache[entry->hash % table->cache_size] == entry)
            table->cache[entry->hash % table->cache_size] = entry->alternative;

        // get the slice
        tableslice *slice = table->entries[entry->hash % table->width];

        // rewire the entries, remove entry from linked list
        if((*entryptr = entry->alternative) == NULL) {
            if(entryptr >= slice->array) {
                size_t index = entryptr - slice->array;
                if(index < slice->size) {
                    // remove slice entry at index
                    memcpy(slice + index, slice + (index + 1), slice->size - (index + 1));

                    // decrement slice size
                    slice->size--;
                }
            }
        }

        // free the entry
        _free_tableentry(entry);
    }
}

#define _NO_INIT_ALTERNATIVE 2
static inline tableentry *_find(hashtable *table, char *key) {
    // check if the key should be shadowed
    char shadow;
    if((shadow = (*key == '#')))
        key++;

    _H_HASH hash = _hash(key);
    tableentry *value;

    // cache lookup if not shadowing
    if(!shadow) {
        value = table->cache[hash % table->cache_size];

        // check if key matches
        if(value != NULL && strcmp(key, value->key) == 0) {
            return value;
        }
    }

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
    size_t index = ((uint64_t) (top = slice->size) * hash) / ((uint64_t) _H_MAX_VALUE + 1);

    #if _H_INTERPOLATION_SEARCH
        _H_HASH top_value = _H_MAX_VALUE, bottom_value = 0;
    #endif

    // binary search
    tableentry *new_value;
    while(top > bottom) {
        value = slice->array[index];

        #if _H_INTERPOLATION_SEARCH
        // if hashes don't match, perform interpolation

        if(value->hash < hash) {
            bottom = index + 1;
            bottom_value = value->hash;

            // the new range is now ]index, top[
            index = (top - index -1) * 
                (hash - value->hash) / (top_value - value->hash) + index +1;
        } else if(value->hash > hash) {
            top = index;
            top_value = value->hash;

            // the new range is now [bottom, index[
            index = (index - bottom) *
                (hash - bottom_value) / (value->hash - bottom_value) + bottom;
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

            // this is for shadowing
            tableentry **source = slice->array + index;

            // compare keys
            while(strcmp(key, value->key)) {
                // walk linked-list
                if(value->alternative == NULL) {
                    // not found, allocate

                    // attach at end of chain
                    value->alternative = new_value = malloc(sizeof(tableentry));

                    // register into stack
                    *(tableentry ***)push(table->stack, &table->offset,
                            sizeof(tableentry **)) = &value->alternative;

                    // initialize entry
                    goto init_entry;
                }
                source = &value->alternative;
                value = value->alternative;
            }

            // shadow if requested
            if(shadow) {
                // backup the current value of *source
                new_value = value;

                // create new tableentry at source
                *source = malloc(sizeof(tableentry));

                // put it onto the stack
                // since the entry to be shadowed already exists, 
                // it will be removed from stack after this
                // so ht_down is actually safe
                *(tableentry ***)push(table->stack, &table->offset,
                        sizeof(tableentry **)) = source;

                // set the alternative
                (*source)->alternative = value;

                // set new_value for initialization
                new_value = *source;

                // register into cache
                table->cache[hash % table->cache_size] = new_value;

                // make sure not to overwrite the alternative
                shadow = _NO_INIT_ALTERNATIVE;

                // initialize entry
                goto init_entry;
            }

            // register entry in cache
            table->cache[hash % table->cache_size] = value;

            // return the found data
            return value;
        }

    }

    // hash not found, insert at requested position
    value = slice->array[index];

    // get insertion index
    index = ((value != NULL && value->hash <= hash) ? (index + 1) : (index));

    // check if a new element fits
    if(slice->size + 1 > slice->size_allocated) {
        // allocate new slice
        tableslice *newslice =
            malloc(sizeof(tableslice) + slice->size_allocated * 2 * sizeof(tableentry *));
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

        // push address stack
        *(tableentry ***)push(table->stack, &table->offset,
                sizeof(tableentry **)) = &newslice->array[index];
    } else {
        // move everything after index one to the right
        memcpy(slice->array + (index + 1), slice->array + index,
                (slice->size - index) * sizeof(tableentry *));

        // insert at index into array
        slice->array[index] = (new_value = malloc(sizeof(tableentry)));

        // push address stack
        *(tableentry ***)push(table->stack, &table->offset,
                sizeof(tableentry **)) = &slice->array[index];

        // increment slice size
        slice->size++;
    }

    // init new element and return it
    init_entry:

    new_value->key = strdup(key);
    new_value->hash = hash;

    if(shadow != _NO_INIT_ALTERNATIVE)
        new_value->alternative = NULL;

    // allocate the wanted mementry and set default value
    (new_value->entry = malloc(sizeof(mementry)))->type = UNDEFINED;
    new_value->entry->value = NULL;
    new_value->entry->level = table->level;
    new_value->entry->flags = 
        (struct s_mementry_flags) {.from_ht=1, .val_tmp=0};

    // register entry in cache
    table->cache[hash % table->cache_size] = new_value;

    // return new entry
    return new_value;

}

void walk_table(hashtable *table, void (*callback)(tableentry *)) {
    // iterate slices
    tableslice *sliceptr;
    tableentry *entry;
    for(size_t i = 0; i < table->width; i++){
        sliceptr = table->entries[i];

        // iterate entries
        if(sliceptr != NULL)
            for(size_t j = 0; j < sliceptr->size; j++) {
                entry = sliceptr->array[j];
                while(entry != NULL) {
                    callback(entry);
                    entry = entry->alternative;
                }
            }

    }
}

mementry *find(hashtable *table, char *key) {
    char *tst;
    char *next = tst = key = strdup(key);
    char done = 0;

    mementry *ret = NULL;

    while(1) {
        // go to next point
        while(*next != '.') {
            if((done = (*next == '\0')))
                break;
            next++;
        }

        *next = (char)'\0';


        // find child in the parent object
        ret = _find(table, key)->entry;

        if(done || ret == NULL)
            break;

        // just allocate it as as an object
        if(ret->type == UNDEFINED) {
            ret->type = OBJECT;
            ret->value = create_hashtable(8, 4);
        }

        if(ret->type != OBJECT)
            break;

        // make the result the new parent object
        table = (hashtable *)ret->value;

        // make sure to search for the child's key
        key = ++next;
    }
    free(tst);
    return ret;
}

void free_mementry(mementry *entry) {
    // TODO: free objects properly
    // free the value
    free(entry->value);

    // free the entry itsself
    free(entry);
}
