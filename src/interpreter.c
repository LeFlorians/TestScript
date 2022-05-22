#include "interpreter.h"
#include "parser.h"

/*
* The interpreter provides an environment for code to run
*/

// interpret from a stream
void f_interpret(environment *memory, FILE *stream) {
    stnode *root;

    tkncache *cache = gencache(stream);

    int level = 1;

    do {
        printf("\nNew Parse tree:\n");
        root = parse(cache);

        printst(root);

        if(root->type == BLOCK)
            level++;
        else if(root->type == BLOCK_END)
            level--;

    } while (level != 0 && root->type != ERROR);

    printf("Reached end of file!\n");
}
