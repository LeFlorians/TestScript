#include "libraries.h"

char callfunc(cfunction *func, mementry *params, mementry *dst, errorinfo *info) {
    
    // call wfunction and return
    ((wfunction)(func->funptr))(params, dst, info);
}
