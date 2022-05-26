#include <stdlib.h>

#include "memory/stack.h"
#include "tokenizer.h"
#include "mapop.h"

typedef enum {
    ERROR,
    FILE_END,
    BLOCK, 
    BLOCK_END,
    MEMBER, 
    CALL,
    EMPTY, 
    INDEX,
    EXPR,   
    VALUE,
} nodetype;

// Defines a tree node
typedef struct stnode stnode;

struct stnode {
    nodetype type;

    union {
        // for any non-leaf node type
        struct {
            stnode *left, *right;
            operator *op;
        } parent;

        // only for VALUE type
        struct {
            char *value;

        } leaf;
    } data;


};

// For simplification
typedef struct {
    token *cur, *peek;
    FILE *input;

    /*
        Stack for bracket matching
        ! This stack will not deal with/contain any curly brackets, the interpreter has to count those.
    */
    stack *bracketstack;
} parsercache;

// Generate a cache for parser
parsercache *gencache(FILE *input);

// Function to parse a stream and save resulting tree in dst
// @param cache Cache created by gencache()
stnode *parse(parsercache *cache);

// Function to print a tree to a stream
void printst(stnode *root);