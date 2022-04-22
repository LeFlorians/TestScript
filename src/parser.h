#include "tokenizer.h"

typedef enum {
    BLK, // Block, contain many operator nodes
    OP, // contain multiple arguments to operator
    CONST, // constant in-place value
    VAR, // Field reference
} stnode_type;

// Define a tree node
typedef struct stnode stnode;

struct stnode {
    stnode_type type;
    stnode *child, *sibling;

    token token;
};

// Parser memory struct
typedef struct {
    stnode *op, *val; // pointers to current operator/value node
} parsermem;

// Define a syntax tree struct
typedef struct {
    stnode root; // root node
} syntaxtree;

// Function to append token to tree
void appendtkn(syntaxtree *tree, parsermem *mem, token *tkn);

// function to init the tree and memory
void init_syntaxtree(syntaxtree *tree, parsermem *mem);