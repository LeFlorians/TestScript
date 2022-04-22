#include <stddef.h>
#include <stdbool.h>

/*
* This file declares the stack memory structure
*/

// Should only be interacted with using the provided funcitons
// Can be instantiated using create_stack()
typedef struct stack {
    size_t alloc_size;
    void *start, *end, *current; // end is exclusive
} stack;

// define stack functions
stack *create_stack(size_t initial_size);
void free_stack(stack *st); // also frees values!

void *push(stack *st, void *val); // 

void *pop(stack *st);
void *peek(stack *st);
void *get_element(size_t element); // returns NULL if out-of-bounds

bool is_empty(stack *st);
size_t get_size(stack *st);