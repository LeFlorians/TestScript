#include <stdlib.h>

#include "tokenizer.h"

// All the operators in ascending binding-power order
typedef enum {
    OP_NULL, // internal, lowest precedence
    LIST, // ,
    ADD,
    SUB,
    MUL,
} operator;

typedef enum {      // n_children   represents
    BLOCK_END,      // 0            }
    MEMBER,         // 1-2          expr (+ member)
    FUNC_END,       // 0            )
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