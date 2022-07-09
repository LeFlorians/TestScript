// Foreign function interface library
#include <ffi.h>

#include "../../error/error.h"
#include "../../memory/hashtable.h"

// an interface to define cfunctions through c-wrapper-files
// internally used for the stdlib implementation
typedef void (*wrapfunction)(mementry *args, mementry *dst);

// define a foreign function
typedef struct {
    ffi_cif cif; // call interface

    void *funptr; // pointer to the function
} foreignfunction;

// define a cfunction
typedef struct {
    // if 1, funptr is of type wrapfunction
    //      with predefined types (as stated above)
    // if 0, funptr and cif refer to a foreign function
    char wrapped;

    void *funptr; // function pointer

    errorinfo *info; // errorinfo
} cfunction;

// create a foreign function
// returns 1 on error
char loadfunc(cfunction *dst, errorinfo *info, char *filename, char *funcname,
        unsigned nargs, ffi_type **args, ffi_type *rtype);

// call a foreign function with given arguments and return pointer
// and convert each argument and return type into the correct datatype
char callfunc(cfunction *func, mementry *params, mementry *dst);