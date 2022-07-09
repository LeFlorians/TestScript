#include <stdio.h>

#include "libraries.h"
#include "../../memory/array.h"

// load a function from a shared object file
char loadfunc(cfunction *dst, errorinfo *info, char *filename, char *funcname,
        unsigned nargs, ffi_type **args, ffi_type *rtype){

    foreignfunction *ff;

    dst->funptr = ff = malloc(sizeof(foreignfunction));

    // make sure this is recognized as a foreign function
    dst->wrapped = 0;

    // !------ POSIX/UNIX API ------
    #ifndef _WIN32

    #include <stddef.h>
    #include <dlfcn.h>

        // opan shared object
        void *sdl = dlopen(filename, RTLD_LAZY);
        if(sdl == NULL){
            // throw error and return
            throw(EF_DL_LOADING_FAILED, info);

            // print error message
            if(info->throwable)
                printf("dlerror: %s\n", dlerror());

            return ERROR;
        } 

        // load function pointer and check for errors
        if((ff->funptr = dlsym(sdl, funcname)) == NULL) {
            // throw error and return
            throw(EF_FF_LOADING_FAILED, info);

            // print error message
            if(info->throwable)
                printf("dlerror: %s\n", dlerror());

            return ERROR;
        }

    // !------ Windows ------
    #else 
        // TODO: implement

        #include <windows.h>

        void loadlib() {
            HMODULE sdl = LoadLibrary(TEXT(filename), RTLD_LAZY); 
            if(sdl == NULL){ // TODO: report error
            } else {
                
            }
        }

    #endif

    // prepare the function
    if(ffi_prep_cif(&ff->cif, FFI_DEFAULT_ABI,
        nargs,  // number of arguments
        rtype,  // return type
        args    // array of argument types
    ) != FFI_OK) {
        // throw error and return
        throw(EF_FFI_ERROR, info);

        return ERROR;
    };

    
    // succeed
    return OK;
}

// call a foreign function
char callfunc(cfunction *func, mementry *params, mementry *dst) {
    
    // check if its a c-wrapped-function
    if(func->wrapped) {
        // call it and return
        ((wrapfunction)(func->funptr))(params, dst);
        return OK;
    }

    foreignfunction *ff = func->funptr;

    if(ff->cif.nargs == 0) {
        if(params != NULL) {
            // TODO: throw error: there should not be arguments

            return ERROR;
        }
    } else {
        if(params == NULL) {
            // TODO: throw error
            return ERROR;
        }

        if((ff->cif.nargs == 1) ^ (params->type != TUPLE)) {
            // dst is not of the right type
        }
    }


    // break if not enough arguments
    if(dst != NULL && (dst->type == ARRAY || dst->type == TUPLE)) {
        if(((array *)dst->value)->size != ff->cif.nargs) {
            // TODO: throw error: not right amount of arguments
            return ERROR; 
        }
    }
    // allocate a pointer array for arguments
    void **args = malloc(ff->cif.nargs * sizeof(void *));

    // allocate space for each argument and put them in place
    for(size_t i = 0; i < ff->cif.nargs; i++) {
        // allocate space
        *(args + i) = malloc((*(ff->cif.arg_types + i))->size);

        // copy value
        if(ff->cif.nargs == 1) {
            
        } else {
            // it's a tuple
            mementry *element = get((array *)dst, i);

            // TODO: make to right type and put into **(args + i)

        }


    }

    // allocate space for return value
    void *ret = malloc(ff->cif.rtype->size);

    return OK;
}


