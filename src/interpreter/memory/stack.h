#pragma once

#include <stddef.h>

/*
* This file declares the stack memory structure
*/

// Should only be interacted with using the provided funcitons
// Can be instantiated using create_stack()
typedef struct {
    size_t alloc_size;
    char *start, *end, *current; // end is exclusive
} stack;

// define the datatype of the size of a stack element
typedef unsigned char elementsize;

// define stack functions
stack *create_stack(size_t initial_size);
void free_stack(stack *st); // free stack and its allocated memory
void fit_stack(stack *st); // shrink the stack to the exact space it currently occupies

void *push(stack *st, elementsize size); 

void *pop(stack *st, elementsize size);
void *peek(stack *st, elementsize size);
void *get_element(size_t element, elementsize size); // returns NULL if out-of-bounds

char is_empty(stack *st);
size_t get_size(stack *st); // Return amount of items currently on stack