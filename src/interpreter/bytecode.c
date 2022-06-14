#include "bytecode.h"

#define INITIAL_DATA_SIZE 32
#define INITIAL_EXPR_SIZE 4

// see protocol specifications in header file
void _recursiveconsume(bytecode *dst, stnode *subtree) {

    switch(subtree->type){

        case NUMBER:
            // Push a reasonable representation of the value onto the stack
            char *endprtr;

            // ? might check for errors like overflow/underflow
            *((number *) push(dst, sizeof(number))) = strtold(subtree->data.leaf.value, &endprtr);
            
            // free the text, since all the required data is now stored in number format
            free(subtree->data.leaf.value);

            // Push datatype
            *((char *)push(dst, 1)) = NUMBER; // this is a number

            break;

        case FIELD:
        case STRING:
            // push pointer and don't free that memory
            *((char **) push(dst, sizeof(char **))) = subtree->data.leaf.value;

            // Push Type
            if(subtree->type == FIELD)
                *((char *)push(dst, 1)) = FIELD; // this is a field
            else
                *((char *)push(dst, 1)) = STRING; // this is a string
            
            // TODO: may reallocate space for string

            break;

        case BLOCK:
            *((char *) push(dst, 1)) = -OP_BLOCK-1;
            break;

        case BLOCK_END:
            *((char *) push(dst, 1)) = -OP_BLOCK_END-1;
            break;

        case EXPR:
            // process left child first
            _recursiveconsume(dst, subtree->data.parent.left);

            // then right child
            if(subtree->data.parent.right != NULL)
                _recursiveconsume(dst, subtree->data.parent.right);

            // then push operator onto the stack
            *((char *) push(dst, 1)) = -subtree->data.parent.op->opcode-1;
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
    
    // allocate the bytecode
    bytecode *code = create_stack(INITIAL_DATA_SIZE);

    // post order traversal
    _recursiveconsume(code, root);

    // return code
    return code;
}
