#include "libraries.h"

char callfunc(cfunction *func, mementry *params, mementry *dst) {
    
    // call wfunction and return
    ((wfunction)(func->funptr))(params, dst);
}