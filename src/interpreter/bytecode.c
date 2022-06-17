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
            char** keyptr = ((char **) push(dst, sizeof(char **)));

            if(keyptr == NULL){
                // TODO: throw another error
                printf("Memory could not be reallocated on stack.\n");
                return;
            }

            *keyptr = subtree->data.leaf.value;


            // Push Type
            if(subtree->type == FIELD)
                *((char *)push(dst, 1)) = FIELD; // this is a field
            else
                *((char *)push(dst, 1)) = STRING; // this is a string
            
            // ? may reallocate space for string

            break;

        case BLOCK:
            *((char *) push(dst, 1)) = OP_BLOCK | (char)128; // set 8th bit
            break;

        case BLOCK_END:
            *((char *) push(dst, 1)) = OP_BLOCK_END | (char)128;
            break;

        case EXPR:
            // if there is one, process right child first
            if(subtree->data.parent.right != NULL)
                _recursiveconsume(dst, subtree->data.parent.right);

            // then process left child
            _recursiveconsume(dst, subtree->data.parent.left);

            // then push operator onto the stack
            *((char *) push(dst, 1)) = subtree->data.parent.op->opcode | (char)128;
            break;

        case MEMBER:
            // TODO: allocate the member lambda function somewhere else and create a link

            // remember the current stack offset (because it may resize)
            size_t offset = dst->current - dst->start;

            // Consume a member chain into the stack
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

            // calculate function size and allocate space for it in a new stack
            size_t funcsize = dst->current - dst->start - offset;

            bytecode *function = create_stack(funcsize); 

            // copy over the function
            char *save = dst->start;
            for(char *funcdst = (char *) push(function, funcsize); save < dst->current; save++)
                *(funcdst++) = *save;
            
            // restore stack offset
            dst->current = dst->start + offset;

            // push function pointer to stack
            *((bytecode **) push(dst, sizeof(bytecode *))) = function;

            // Push datatype
            *((char *)push(dst, 1)) = FUNCTION; // this is a function

            break;

        case FILE_END:
            // Do not push anything
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
