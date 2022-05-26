#include <stdlib.h>

#include "tokenizer.h"
#include "mapop.h"

// TODO: replace the most used non-kids node with NULL
typedef enum {
    FILE_END,   // no kids
    BLOCK,  // no kids
    BLOCK_END, // no kids
    MEMBER, 
    CALL,
    EMPTY,  // no kids
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

        struct {
            char *value;

        } leaf;
    } data;


};

// Parse a stream and save resulting tree in dst
stnode *parse(cache *cache);

// Read a new token and keep track of read brackets
void advance(cache *cache);