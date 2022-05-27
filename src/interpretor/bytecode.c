#include "bytecode.h"

#define INITIAL_STACK_SIZE 8

void _recursiveconsume(stack *dst, stnode *subtree) {

    if(subtree->type == NUMBER) {
        // Push a reasonable representation of the value onto the stack

        

    } else {
        // process left child first
        _recursiveconsume(dst, subtree->data.parent.left);

        // then right child
        _recursiveconsume(dst, subtree->data.parent.right);

        // then push operator onto the stack
    }
    
}

bytecode *consume(stnode *root) {
    
    // allocate a stack to write the bytecode to
    // TODO: could be optimized using another stack type
    stack *stk = create_stack(1, INITIAL_STACK_SIZE);

    // Post order traversal
    bytecode *dst;

}
