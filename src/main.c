#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "tokenizer.h"

int main(int argc, char** argv){

    // For later: https://www.gnu.org/software/libc/manual/html_node/Streams-and-Threads.html
    FILE *input;

    if(argc >= 2) {
        // open a file stream
        input = fopen(argv[1], "r");
    } else {
        // Just read from standard input
        input = stdin;
    }

    token tkn;
    do {
        readtkn(input, &tkn);

        printf("Type: %i ", tkn.type);

        switch(tkn.type){
            case INTEGER:
                printf("Integer value: %lli\n", tkn.num.integer);
                break;
            case NUMBER:
                printf("Floating-point value: %lf\n", tkn.num.number);
                break;
            case NULLTKN:
                printf("Reached NULLTKN");
                break;
            default:
                printf("Content: %s\n", tkn.content);
                break;
        }


    } while(tkn.type != INVALID && tkn.type != NULLTKN);

}