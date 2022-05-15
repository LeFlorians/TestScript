#include "interpreter.h"
#include "parser.h"

/*
* The interpreter provides an environment for code to run
*/

// interpret from a stream
void f_interpret(environment *memory, FILE *stream) {

    stnode* root = parse(stream);

    printf("%i", root->type);

    printf("\nPrinting tree:\n");
    printst(root);

}
