#include <stdio.h>
#include <string.h>

#include "interpreter.h"

/*
* The interpreter provides an environment for code to run
*/

// interpret from a stream
void f_interpret(environment *memory, FILE *stream) {
    // -- Step 1: read tokens & build syntax tree

    token tkn;

    while(1) {
        readtkn(stream, &tkn);

        printf("type %i @ %li:%li ", tkn.type, tkn.info.line, tkn.info.character);

        switch(tkn.type){
            case NULLTKN:
                printf("NULLTKN\n");
                break;
            default:
                printf("content: %s\n", tkn.content);
                break;
        }

        if(tkn.type == INVALID || tkn.type == NULLTKN)
            break;
    }

}

void interpret(environment *memory, char *text) {
    FILE *stream;
    
    stream = fmemopen(text, strlen(text), "r");
    f_interpret(memory, stream);
}

