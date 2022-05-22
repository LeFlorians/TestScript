#include <stdlib.h>

#include "stack.h"

#define DEFAULT_ALLOC_SIZE 8

#define MAX_ALLOC_SIZE 0xFFFF

/*
    This private function resizes the stack.

    When resizing fails, the stack will **keep its current size**,
    the stack top may be lowered if the stack was supposed to be shortened.
*/
void _resize_stack(stack *st, size_t newsize) {
    char *newptr;

    // save offset
    size_t offset = st->current - st->start;

    // resize array
    if((newptr = realloc(st->start, newsize)) != NULL) {
        st->start = newptr;

        st->end = st->start + newsize;
    }
    
    // reset current pointer
    if(offset < newsize)
        st->current = st->start + offset;
    else
        st->current = st->end - 1;
}

stack *create_stack(size_t element_size, size_t size){
    stack* ret = malloc(sizeof(stack));
    ret->start = malloc(element_size * size);
    ret->end = ret->start + size * element_size;
    ret->current = ret->start - element_size;

    ret->element_size = element_size;
    ret->alloc_size = DEFAULT_ALLOC_SIZE;

    // initialize with 0s
    for(char **e = &(ret->start); e < &(ret->start); e++)
        *e = 0;

    return ret;
}

void free_stack(stack *st) {
    // free allocated memory
    free(st->start);

    // free stack struct
    free(st);
}

// Pushing NULL will be ignored
// Returns val if push was successful, NULL otherwise
// @param val Should be at least of size st->element_size
void *push(stack *st, void *val) {

    // ignore NULL as stated
    if (val == NULL)
        return NULL;

    char *charpointer = (char *) val;

    if(st->current + st->element_size >= st->end){
        // reached the end, reallocate

        size_t new_size = st->end - st->start + st->alloc_size * st->element_size;

        // new_size is too large
        if(new_size < st->alloc_size){
            return NULL;
        }

        _resize_stack(st, new_size);

        if(st->current + 1 >= st->end) {
            // reallocation failed
            return NULL;
        }

        // next time, automatically allocate more
        // TODO: better new_size prediction
        if(st->alloc_size < MAX_ALLOC_SIZE)
            st->alloc_size <<= 1;
    }

    // move current
    st->current += st->element_size;

    // push value onto stack
    for(size_t index = 0; index < st->element_size; index++)
        st->current[index] = charpointer[index];
        
    return val;
}

// Returns the element on top of the stack (or NULL if empty) and removes it
void *pop(stack *st) {
    // check if not empty
    if(st->current >= st->start){
        void *ret = st->current;
        st->current -= st->element_size;
        return ret;
    }
    return NULL;
}

// Returns element on top of the stack or NULL if empty
void *peek(stack *st) {
    if(st->current >= st->start)
        return st->current;
    return NULL;
}

// Returns true if empty, false otherwise
bool is_empty(stack *st) {
    return st->current < st->start;
}

// Returns the current amount of items on the stack
size_t get_size(stack *st) {
    return (st->current - st->start) / st->element_size + 1;
}
