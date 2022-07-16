#pragma once

#include <stddef.h>

/*
* This file declares the stack memory structure
*/

// Should only be interacted with using the provided funcitons
// Can be instantiated using create_stack()
typedef struct {
    size_t size; // allocated size
    size_t elements; // number of elements on the stack
    void *floor; // pointer to the stack data
} stack;

// type of a stack pointer (offset from start, initialize this to zero)
typedef size_t *stackptr;

// define the datatype of the size of a stack element
typedef unsigned char elementsize;

// define stack functions
stack *create_stack(size_t initial_size);
void free_stack(stack *st); // free stack and its allocated memory
void fit_stack(stack *st); // shrink the stack to the exact space it currently occupies

void *push(stack *st, stackptr ptr, elementsize size); 

void *pop(stack *st, stackptr ptr, elementsize size);
void *peek(stack *st, stackptr ptr, elementsize size);
void *get_element(size_t element, elementsize size); // returns NULL if out-of-bounds