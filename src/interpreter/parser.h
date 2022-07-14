#include <stdlib.h>

#include "tokenizer.h"
#include "mappings/mapop.h"

// Defines a tree node
typedef struct stnode stnode;

// Define stnode types
typedef typing nodetype;

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
void parser_init(cache *cache);
