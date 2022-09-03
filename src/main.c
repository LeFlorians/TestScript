#include <stdio.h>
#include <unistd.h>

#include "interpreter/interpreter.h"

int main(int argc, char** argv){
    FILE *input;

    if(argc >= 2) {
        // open a file stream
        input = fopen(argv[1], "r");
    } else {
        // Just read from standard input
        input = stdin;
    }

    // interpret input
    interpret(input, argc >= 2 ? argv[1] : NULL);
}
