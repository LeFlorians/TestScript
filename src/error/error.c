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
    "The specified operator cannot be applied to given operand(s)",
    "The specified operand is not a function, therefore it cannot be called",

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