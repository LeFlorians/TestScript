#include "interpreter.h"
#include "tokenizer.h"

/*
* The interpreter provides an environment for code to run
*/

// interpret from a stream
void interpret(FILE *stream, char* filename) {
    token tkn;
    cache cac;

    // setup the cache
    {
        cac.input = stream;
        cac.cur = &tkn;

        // enable error messages
        cac.info.throwable = 1;

        // set defaults for errors
        cac.info.fileinfo.filename = filename;
        cac.info.fileinfo.character = 0;
        cac.info.fileinfo.line = 1;
    }

    static char* typenames[] = {"Nulltkn", "Bracket", "Field", "Symbol", "String", "Number"};

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
