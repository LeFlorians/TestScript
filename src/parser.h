#include <stdlib.h>

#include "tokenizer.h"
#include "mapop.h"

typedef enum {      // n_children   represents
    BLOCK_END,      // 0            }
    MEMBER,         // 1-2          expr (+ member)
    FUNC_END,       // 0            )
    EXPR,           // 1-2 & op     any expression
    VALUE,
} nodetype;
const char* typeNames[] = {
    "BlockEnd", "Member", "FuncEnd", "Expr", "Value",
};

// Defines a tree node
typedef struct stnode stnode;

struct stnode {
    nodetype type;

    union {
        // for any non-leaf node type
        struct {
            stnode *left, *right;
            operator op;
        } parent;

        // only for VALUE type
        struct {
            char *value;

        } leaf;
    } data;


};

// Function to parse a stream and save resulting tree in dst
stnode *parse(FILE* input);

// Function to print a tree to a stream
void printst(stnode *root);