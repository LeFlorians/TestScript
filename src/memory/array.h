#pragma once

#include <stdlib.h>

#include "hashtable.h"

typedef struct {
    mementry **arr;

    // start: index of first element relative to arr
    // size: number of elements currently stored in arr
    // size_allocated: size of memory allocated for arr
    size_t size, size_allocated;
} array;

// create/free array
array *create_array(size_t initial_size);
void free_array(array *arr);

// set elements of array at given array index (override)
char set(array *arr, array *element, size_t index);
// set element at given array index
char set_element(array *arr, mementry *element, size_t index);

// get element at given index or NULL if out of bounds
mementry *get(array *arr, size_t index);