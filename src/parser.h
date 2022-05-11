#include <stdlib.h>

#include "tokenizer.h"

// All the operators in ascending binding-power order
typedef enum {
    LIST, // ,
    ADD,
    SUB,
    MUL,
} operator;

typedef enum {      // n_children   represents
    BLOCK,          // 1            { (start of new scope)
    BLOCK_END,      // 0            }
    MEMBER,         // 1-2          expr (+ member)
    FUNC,           // 1            ( function call; left: expr
    FUNC_END,       // 0            )
    EXPR,           // 1-2 + op     expression;
    VALUE,
} nodetype;

// Defines a tree node
typedef struct stnode stnode;

struct stnode {
    nodetype type;
    stnode *left, *right;
    operator op;

    // Only for VALUE type
    char *value;
};

// Function to allocate stnode
stnode  *allocate_stnode() {
    // TODO: set default values
    return (stnode *)malloc(sizeof(stnode));
}

// Function to parse a stream and save resulting tree in dst
void parse(FILE* input, stnode *dst);