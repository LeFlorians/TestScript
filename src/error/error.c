#include <stdio.h>

#include "error.h"

static const char *errformats[] = {
    #ifndef MEAN
    // Nice error messages
    "Quotation marks not closed",
    "Block comment not closed",
    "Invalid escape sequence"
    
    #else
    // Not nice error messages
    "",

    #endif
};

void throw(enum errorformat format, errorinfo *info) {
    if(info->throwable) {
        fprintf(stderr, "[%i:%i] %s\n", 
            info->fileinfo.line, info->fileinfo.character, errformats[format]);   
    }
}