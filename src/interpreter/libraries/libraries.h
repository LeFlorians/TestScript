#include "../memory/hashtable.h"

// type of a 'wrapped' function
typedef void (*wfunction)(mementry *args, mementry *dst);

// definition of a c-function
typedef struct {
    // TODO: expand to foreign functions at some point

    // pointer to the function (wfunction *)
    void *funptr;
} cfunction;


// function to call a cfunction
char callfunc(cfunction *func, mementry *params, mementry *dst);