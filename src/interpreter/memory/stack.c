#include <stdlib.h>
#include <string.h>

#include "stack.h"
/*
    This private function resizes the stack.

    When resizing fails, the stack will **keep its current size**,
    the stack top may be lowered if the stack was supposed to be shortened.
*/
#define MIN(x, y) (((x) < (y)) ?  (x) : (y))
static inline void _resize_stack(stack *st, size_t newsize) {
    void *newptr;

    // resize array
    if((newptr = malloc(newsize)) != NULL) {
        // copy data
        memcpy(newptr, st->floor,  MIN(st->elements, newsize));

        // free old stack pointer
        free(st->floor);

        // reassign pointer and size
        st->floor = newptr;
        st->size = newsize;
    }
}

stack *create_stack(size_t size){
    stack* ret = malloc(sizeof(stack));
    ret->floor = malloc(size);
    ret->size = size;

    // initialize with 0
    memset(ret->floor, 0, size);

    return ret;
}

void free_stack(stack *st) {
    // free allocated memory
    free(st->floor);

    // free stack struct
    free(st);
}

// Returns pointer to allocated space if push was successful, NULL otherwise
void *push(stack *st, stackptr ptr, elementsize size) {

    // Resize stack if size is not sufficient
    if(st->size < *ptr + size){
        // reached the end, reallocate

        size_t new_size = st->size * 2;

        // Just use total size if too small
        if(new_size < *ptr + size)
            new_size = *ptr + size;

        _resize_stack(st, new_size);

        // check if reallocation was successful
        if(st->size < *ptr + size) {
            // TODO: error
        }
    }

    // pointer to return
    void *ret = st->floor + *ptr;

    // shift stack pointer
    *ptr += size;

    // set number of elements
    st->elements = ((st->elements) > (*ptr) ? st->elements : *ptr);
        
    return ret;
}

// Returns the element on top of the stack (or NULL if empty) and removes it
void *pop(stack *st, stackptr ptr, elementsize size) {
    // check if not empty
    if(st->elements + size >= *ptr){
        // decremt pointer and return stack element
        return (st->floor + (*ptr -= size));
    }
    return NULL;
}

// Returns element on top of the stack or NULL if empty
void *peek(stack *st, stackptr ptr, elementsize size) {
    if(st->elements >= *ptr + size)
        return (st->floor + (*ptr - size));
    return NULL;
}

void fit_stack(stack *st) {
    if(st->size > st->elements)
        _resize_stack(st, st->elements);
}