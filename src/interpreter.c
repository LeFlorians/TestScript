#include "interpreter.h"
#include "tokenizer.h"

/*
* The interpreter provides an environment for code to run
*/

// interpret from a stream
void f_interpret(environment *memory, FILE *stream) {
    token tkn;

    tokencache cac;
    cac.input = stream;
    cac.cur = &tkn;

    cac.info.line = 1;
    cac.info.character = 0;

    static char* typenames[] = {"Nulltkn", "Invalid", "Bracket", "Field", "Symbol", "String", "Number"};

    do {
        readtkn(&cac);
        printf("Type: %s Value: (%s)\n", typenames[tkn.type], tkn.content);
    } while(tkn.type != 0);


    return;

    // stnode *root;

    // parsercache *cache = gencache(stream);

    // do {
    //     printf("\nNew Parse tree:\n");
    //     root = parse(cache);

    //     printst(root);

    // } while (root->type != ERROR && root->type != FILE_END);

    // printf("Reached end of file!\n");

    // // Print unmatched brackets
    // size_t size;
    // if((size = get_size(cache->bracketstack)) != 0) {
    //     printf("\nNot all brackets matched, %li left!\n", size);
    //     printf("Assuming: ");
    //     for(size_t index = size; index > 0; index--)
    //         putchar(cache->bracketstack->start[index - 1]);
    //     putchar('\n');
    // }
}
