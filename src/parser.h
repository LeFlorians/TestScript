#include <stdlib.h>

#include "tokenizer.h"
#include "mapop.h"

typedef enum {      // n_children   represents
    ERROR,          //              parser error, message in data.leaf.value
    BLOCK, 
    BLOCK_END,      // 0            }
    CALL,           //              function call
    INDEX,          //              array index
    EXPR,           // 1-2 & op     any expression
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
} tkncache;

// Generate a cache for parser
tkncache *gencache(FILE *input);

// Function to parse a stream and save resulting tree in dst
// @param cache Cache created by gencache()
stnode *parse(tkncache *cache);

// Function to print a tree to a stream
void printst(stnode *root);