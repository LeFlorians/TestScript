#include <stdio.h>

/*
    Interpreter toolchain:

          Input
            |
            V
        Tokenizer
            |
            V
          Parser
            |
            V
     Bytecode compiler  (-> Save to File)
            |
            V
         Runtime
*/

// define interpreter methods
void interpret(FILE *stream, char *filename);
