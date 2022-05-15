#include <stdlib.h>

#include "tokenizer.h"

/*
! Here is a table defining operator precedences and associativity
* Similar to C Operator Precedences

| Precedence | Associativity | Operators | Symbol | Position |
| ---------- | ------------- | --------- | ------ | -------- |
| 1          | Left to Right | INC       | ++     | Postfix  |
|            |               | DEC       | --     |          |
| 2          | Right to Left | PIN       | ++     | Prefix   |
|            |               | PDE       | --     |          |
|            |               | POS       | +      |          |
|            |               | NEG       | -      |          |
|            |               | NOT       | !      |          |
| 3          | Left to Right | MUL       | *      | Infix    |
|            |               | DIV       | /      |          |
|            |               | MOD       | %      |          |
| 4          |               | ADD       | +      |          |
|            |               | SUB       | -      |          |
| 5          |               | LET       | <      |          |
|            |               | LEQ       | <=     |          |
|            |               | GRT       | >      |          |
|            |               | GEQ       | >=     |          |
| 6          |               | EQU       | ==     |          |
|            |               | NEQ       | !=     |          |
| 7          |               | AND       | &&     |          |
| 8          |               | ORO       | ||     |          |
| 9          |               | CON       | ?:     | Special  | <- not yet implemented
| 10         | Right to Left | ASS       | =      | Infix    |
|            |               | PEQ       | +=     |          |
|            |               | MEQ       | -=     |          |
|            |               | TEQ       | *=     |          |
|            |               | DEQ       | /=     |          |
|            |               | REQ       | %=     |          |
| 11         | Left to Right | LIS       | ,      |          |

* Operators are defined as follows
* char op = 0b a b ccccccc;
* a: Position:      0=Infix, 1: (LTR: Postfix, RTL: Prefix)
* b: Associativity: 0=LTR, 1=RTL
* c: Operator code (opcode) 

*/

// Therefore, any operator is of char type
typedef char operator;


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