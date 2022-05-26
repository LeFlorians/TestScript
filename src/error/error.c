#include <stdio.h>

#include "error.h"

static const char *errformats[] = {
    #ifndef MEAN
    // Nice error messages
    "Quotation marks not closed",
    "Block comment not closed",
    "Invalid escape sequence",

    "Didn't expect closing bracket",
    "Mismatched bracket",
    "Invalid operator",
    "Expected prefix operator",
    "Expected closing bracket",
    "Expected closing square bracket",
    "Token could not be matched",
    
    #else
    // Not nice error messages
    "",

    #endif
};

void throw(enum errorformat format, errorinfo *info) {
    if(info->throwable) {
        fprintf(stderr, "[%s:%i:%i] %s\n", 
            info->fileinfo.filename == NULL ? "<virtual>" : info->fileinfo.filename,
            info->fileinfo.line, info->fileinfo.character, 
            errformats[format]
        );   
    }
}