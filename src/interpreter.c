#include "interpreter.h"
#include "parser.h"

/*
* The interpreter provides an environment for code to run
*/

// interpret from a stream
void f_interpret(environment *memory, FILE *stream) {
    stnode *root;

    parsercache *cache = gencache(stream);

    do {
        printf("\nNew Parse tree:\n");
        root = parse(cache);

        printst(root);

    } while (root->type != ERROR && root->type != FILE_END);

    printf("Reached end of file!\n");

    // Print unmatched brackets
    size_t size;
    if((size = get_size(cache->bracketstack)) != 0) {
        printf("\nNot all brackets matched, %li left!\n", size);
        printf("Assuming: ");
        for(size_t index = size; index > 0; index--)
            putchar(cache->bracketstack->start[index - 1]);
        putchar('\n');
    }
}
