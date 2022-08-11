#include "interpreter.h"
#include "libraries/stdlib/stdlib.h"
#include "localizer.h"
#include "runtime.h" // only need runtime.h because parser.h, tokenizer.h, etc. are included

/*
* The interpreter provides an environment for code to run
*/

#if DEBUG
// function to print syntaxtree
void printst(stnode *root);
#endif

// interpret from a stream
void interpret(FILE *stream, char* filename) {
    token tkn;
    cache cac;

    // setup the token
    {
        tkn.content = malloc(MAX_CONTENT * sizeof(char));
    }

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

        // allocate bracket stack for parser
        cac.bracketstack = create_stack(16);        
        cac.bracketstack_offset = 0;        
    }

    stnode *root;
    bytecode *code;

    // TODO: choose better table size
    hashtable *memory = create_hashtable(32, 16);

    // load the stdlib
    loadstd(memory);

    // advance once
    parser_init(&cac);

    while(1) {
        // parse tree
        root = parse(&cac);

        #if DEBUG
        // print out tree
        printf("\nNew Parse tree:\n");
        printst(root);
        #endif

        // break if null or FILE_END
        if(root == NULL || root->type == FILE_END)
            break;

        // resolve fields
        localize(root, memory, &cac.info);

        // create bytecode from tree
        code = consume(root);

        // process bytecode, also passing debug information from parser
        process(code, &cac.info);

    };

    #if DEBUG
    printf("Reached EOF\n");
    // Print unmatched brackets
    if(cac.bracketstack_offset != 0) {
        printf("\nNot all brackets matched, %u left!\n", (unsigned int) cac.bracketstack_offset);
        printf("Assuming: ");
        while(cac.bracketstack_offset != 0)
            putchar(*(char *)pop(cac.bracketstack, &cac.bracketstack_offset, 1));
        putchar('\n');
    }
    #endif

    // free token content
    free(tkn.content);
}


#if DEBUG
/* Functions to print out syntaxtree */

void _printst(stnode *root, int depth) {
    // _printside(out, depth);

    static const char* typeNames[] = {
        "Number", "String", "Undefined", "Array", "Tuple",
        "Object", "Function", "CFunction", "Reference", 
        "NullToken", "Bracket", "Symbol", "Field", "FileEnd",
        "Block", "BlockEnd", "Member", "Expr"
    };

    if(root == NULL) {
        printf("(null)\n");
        return;
    }

    printf(". %s", typeNames[root->type]);
    // printf(". %i", root->type);

    if(root->type == EXPR)
        printf(" [%s]", root->data.parent.op->name);
    
    if(root->type == FIELD || root->type == NUMBER || root->type == STRING) {
        printf(" (%s)", root->data.leaf.value);
    } else if(root->type != FILE_END && root->type != BLOCK && root->type != BLOCK_END) {

        if(root->data.parent.left != NULL){
            putchar('\n');
            for(int i = 0; i < depth; i++)
                printf("|");

            putchar('L');

            _printst(root->data.parent.left, depth+1);
        }

        if(root->data.parent.right != NULL){
            putchar('\n');
            for(int i = 0; i < depth; i++)
                printf("|");

            putchar('R');

            _printst(root->data.parent.right, depth+1);
        }
    } 
} 

void printst(stnode *root) {
    _printst(root, 0);
    putchar('\n');
}
#endif