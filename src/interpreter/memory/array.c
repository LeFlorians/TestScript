#include <string.h>

#include "array.h"

array *create_array(size_t initial_size) {
    array *ret = malloc(sizeof(array));
    ret->arr = malloc(initial_size * sizeof(mementry *));

    // set internal values
    ret->size = 0;
    ret->size_allocated = initial_size;

    return ret;
}

void free_array(array *arr) {
    // free elements array
    free(arr->arr);

    // free array itsself
    free(arr);
}

char set(array *arr, array *element, size_t index) {

    // make sure the destination array has the right size
    if(arr->size_allocated < index + element->size){
        size_t newsize = element->size + arr->size;

        if(newsize < arr->size_allocated) {
            // array got too big
            return 0;
        }

        mementry **newptr;
        if((newptr = realloc(arr->arr, newsize)) != NULL) {
            arr->arr = newptr;
            arr->size_allocated = newsize;
        } else {
            // TODO: reallocation failed
            return 0;
        }
    }

    // copy over elements
    memcpy(arr->arr + index, element->arr, sizeof(mementry *) * element->size);

    // increase size
    arr->size += element->size;
    return 1;
}

char set_element(array *arr, mementry *element, size_t index) {

    // make sure the destination array has the right size
    if(arr->size_allocated <= index){
        size_t newsize =  arr->size_allocated << 1;

        if(newsize < arr->size_allocated) {
            // array got too big
            return 0;
        }

        mementry **newptr;
        if((newptr = realloc(arr->arr, newsize)) != NULL) {
            arr->arr = newptr;
            arr->size_allocated = newsize;
        } else {
            // TODO: reallocation failed
            return 0;
        }
    }


    // copy over element
    *(arr->arr + index) = element;

    // increase size
    arr->size++;

    return 1;
}

mementry *get(array *arr, size_t index) {
    if(arr == NULL || index >= arr->size)
        return NULL;
    return *(arr->arr + index);
}
