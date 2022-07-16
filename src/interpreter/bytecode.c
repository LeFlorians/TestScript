#include "bytecode.h"
#include "processing/implementations.h"

#define INITIAL_DATA_SIZE 32
#define INITIAL_EXPR_SIZE 4

// macro to convert a strint to a number
#define DOUBLE(string) (number) strtod(string, NULL)

static inline mementry *_register(bytecode *code, stackptr ptr, typing type) {
    mementry *ret = *(mementry **)push(code, ptr, sizeof(mementry *)) = malloc(sizeof(mementry));
    ret->type = type;
    return ret;
}

// see protocol specifications in header file
void _recursiveconsume(bytecode *dst, stackptr ptr, stnode *subtree, hashtable *table) {

    switch(subtree->type){

        case NUMBER:
            // Push a reasonable representation of the value onto the stack
            *((number *)(_register(dst, ptr, NUMBER)->value = malloc(sizeof(number)))) 
                = DOUBLE(subtree->data.leaf.value);

            // ? might check for errors like overflow/underflow

            // free the text, since all the required data is now stored in number format
            free(subtree->data.leaf.value);

            break;

        case FIELD:
            // find (or allocate) the memory entry and push it to the stack
            _register(dst, ptr, REFERENCE)->value = find(table, subtree->data.leaf.value);
            
            // free the key
            free(subtree->data.leaf.value);
            break;

        case STRING:
            // push pointer and don't free the orignial memory
            _register(dst, ptr, STRING)->value = subtree->data.leaf.value;

            // ? may reallocate space for string

            break;

        case BLOCK:
            // register the block expression
            _register(dst, ptr, EXPR)->value = _block;
            break;

        case BLOCK_END:
            // register the block_end expression
            _register(dst, ptr, EXPR)->value = _block_end;
            break;

        case EXPR:
            // process left child first
            _recursiveconsume(dst, ptr, subtree->data.parent.left, table);

            // if there is one, process right child next
            if(subtree->data.parent.right != NULL)
                _recursiveconsume(dst, ptr, subtree->data.parent.right, table);

            // then push operation onto the stack
            _register(dst, ptr, EXPR)->value = implementationof(subtree->data.parent.op->opcode);
            break;

        case MEMBER:
            // TODO: allocate the member lambda function somewhere else and create a link

            // Create a new stack for the underlying function
            bytecode *function = create_stack(64);

            size_t offset = 0;

            // Consume a member chain into the stack
            stnode *tree;
            while(subtree != NULL && subtree->data.parent.left != NULL) {
                // Consume the left subtree
                _recursiveconsume(function, (stackptr) &offset, subtree->data.parent.left, table);

                // Reassign values for the loop
                tree = subtree;
                subtree = subtree->data.parent.right;

                // Free the MEMBER node
                free(tree);
            }

            // resize the stack to the minimal needed size
            fit_stack(function);

            // push function pointer to stack
            _register(dst, ptr, CODE)->value = function;

            break;

        case FILE_END:
            // Do not push anything
            break;;
    }
    
    // free the subtree node
    free(subtree);
}

bytecode *consume(stnode *root, hashtable *table) {
    
    // allocate the bytecode
    bytecode *code = create_stack(INITIAL_DATA_SIZE);

    // the stack offset
    size_t offset = 0;

    // post order traversal
    _recursiveconsume(code, (stackptr) &offset, root, table);

    // return code
    return code;
}
