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

stack *create_stack(size_t size){
    stack* ret = malloc(sizeof(stack));
    ret->start = malloc(size);
    ret->end = ret->start + size;
    ret->current = ret->start;

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
// Returns pointer to allocated space if push was successful, NULL otherwise
void *push(stack *st, elementsize size) {

    // Resize stack if size is not sufficient
    if(st->current + size > st->end){
        // reached the end, reallocate

        size_t size = st->end - st->start;
        size_t new_size = size + st->alloc_size;

        // Just use total size if too small
<<<<<<< HEAD
        if(st->current + size > st->end + new_size)
            new_size = (st->current - st->end) + size;

        // new_size is too large
        if(new_size < st->alloc_size){
            printf("new size too large\n");
            return NULL;
        }

        _resize_stack(st, new_size);

        // check if reallocation was successful
        if(st->end - st->start == size){
            // Reallocation failed
            return NULL;
        }

=======
        if(st->current + new_size > st->end + size)
            new_size = st->end + size - st->current;

        // new_size is too large
        if(new_size < st->alloc_size){
            return NULL;
        }

        _resize_stack(st, new_size);

        // check if reallocation was successful
        if(st->end - st->start == size){
            // Reallocation failed
            return NULL;
        }

>>>>>>> origin
        // next time, automatically allocate more
        // TODO: better new_size prediction
        if(st->alloc_size < MAX_ALLOC_SIZE)
            st->alloc_size <<= 1;
    }

    // pointer to return
    char *ret = st->current;

    // shift current
    st->current += size;
        
    return ret;
}

// Returns the element on top of the stack (or NULL if empty) and removes it
void *pop(stack *st, elementsize size) {
    // check if not empty
    if(st->current - size >= st->start){
        return (st->current -= size);
    }
    return NULL;
}

// Returns element on top of the stack or NULL if empty
void *peek(stack *st, elementsize size) {
    if(st->current - size >= st->start)
        return (st->current - size);
    return NULL;
}

// Returns true if empty, false otherwise
bool is_empty(stack *st) {
    return st->current < st->start;
}

// Returns the current size used by elements on the stack
size_t get_size(stack *st) {
    return (st->current - st->start);
}
