#include "bytecode.h"

#define INITIAL_STACK_SIZE 32

// Pushes the next byte of information
// TODO: write to a file as well
void _push(stack *dst, unsigned char byte) {
    push(dst, &byte);
}

// see protocol specifications in header file
void _recursiveconsume(stack *dst, stnode *subtree) {

    switch(subtree->type){

        case NUMBER:
            // Push a reasonable representation of the value onto the stack
            char *endprtr;

            // ? might check for errors like overflow/underflow
            number num;
            num.value = strtold(subtree->data.leaf.value, &endprtr);
            
            // Free the text, since all the required data is now in num
            free(subtree->data.leaf.value);

            // push the value
            _push(dst, 0);   // this is a number
            for(char i = 0; i < sizeof(num) * 8; i += 8)
                _push(dst, (num.binary >> i) & 0xFF );
            break;

        case BLOCK:
            _push(dst, 128);
            break;

        case BLOCK_END:
            _push(dst, 192);
            break;;

        case EXPR:
            // process left child first
            _recursiveconsume(dst, subtree->data.parent.left);

            // then right child
            if(subtree->data.parent.right != NULL)
                _recursiveconsume(dst, subtree->data.parent.right);

            // then push operator onto the stack
            _push(dst, 64 | subtree->data.parent.op->opcode);
            break;

        case MEMBER:
            // Consume a member chain
            stnode *tree;
            while(subtree != NULL && subtree->data.parent.left != NULL) {
                // Consume the left subtree
                _recursiveconsume(dst, subtree->data.parent.left);

                // Reassign values for the loop
                tree = subtree;
                subtree = subtree->data.parent.right;

                // Free the MEMBER node
                free(tree);
            }
            break;

        case FILE_END:
            // Do nothing
            break;;
    }
    
    // free the subtree node
    free(subtree);
}

bytecode *consume(stnode *root) {
    
    // allocate a stack to write the bytecode to
    // TODO: could be optimized using another stack type
    stack *stk = create_stack(1, INITIAL_STACK_SIZE);

    // Post order traversal
    _recursiveconsume(stk, root);

    // bytecode to be returned
    bytecode *dst;

    // copy the data and size
    dst->ptr = stk->start;
    dst->size = get_size(stk);

    // free the stack itsself
    free(stk);

    return dst;
}
