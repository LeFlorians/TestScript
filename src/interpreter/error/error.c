#include <stdio.h>

#include "error.h"

static const char *errformats[] = {

    // Nice error messages
    "Max token length exceeded",
    "Quotation marks not closed",
    "Block comment not closed",
    "Invalid escape sequence",

    "Didn't expect closing bracket",
    "Mismatched bracket",
    "Invalid operator",
    "Expected prefix operator",
    "Expected closing bracket",
    "Expected closing square bracket",
    "Token unmatched",

    "Missing argument(s) for given operator",
    "The specified operator cannot be used on of given type(s)",
    "The specified operand is not a function, therefore it cannot be called",
    "The specified operand must be mutable",
    "You can only assign the value from one field to another, if the source "
        "field was created in a scope equal or lower than the destination field",
    "Index out of bounds",

    "Loading of a shared object failed",
    "Loading of foreign function failed",
    "An error occured within libffi when creating a typed foreign function",
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
