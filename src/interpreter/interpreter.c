#include "interpreter.h"
#include "parser.h"

/*
* The interpreter provides an environment for code to run
*/

// function to print syntaxtree
void printst(stnode *root);

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
    }

    // static char* typenames[] = {"Nulltkn", "Bracket", "Field", "Symbol", "String", "Number"};

    // do {
    //     readtkn(&cac);
    //     printf("Type: %s Value: (%s)\n", typenames[tkn.type], tkn.content);
    // } while(tkn.type != 0);

    // return;

    stnode *root;

    // advance once
    advance(&cac);

    do {
        printf("\nNew Parse tree:\n");
        root = parse(&cac);

        printst(root);

    } while (root != NULL && root->type != FILE_END);

    printf("Reached end of file!\n");

    // Print unmatched brackets
    size_t size;
    if((size = get_size(cac.bracketstack)) != 0) {
        printf("\nNot all brackets matched, %lli left!\n", size);
        printf("Assuming: ");
        for(size_t index = size; index > 0; index--)
            putchar(cac.bracketstack->start[index - 1]);
        putchar('\n');
    }
}



/* Functions to print out syntaxtree */

void _printst(stnode *root, int depth) {
    // _printside(out, depth);

    static const char* typeNames[] = {
        "Number", "Field", "String", "Array", "Object", "Nulltkn", "Bracket", "Symbol",
        "FileEnd", "Block", "BlockEnd", "Member", "Expr"
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
    } else if(root->type != FILE_END) {

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
