#include "bytecode.h"
#include "memory/hashtable.h"
#include "processing/implementations.h"

#define INITIAL_DATA_SIZE 32
#define INITIAL_EXPR_SIZE 4

// macro to convert a strint to a number
#define DOUBLE(string) (number) strtod(string, NULL)

static inline mementry *_register(bytecode *code, stackptr ptr, typing type) {
    mementry *ret = 
        *(mementry **)push(code, ptr, sizeof(mementry *)) = _alloc_mementry();
    ret->flags.persistent = 1;
    ret->type = type;
    return ret;
}

void free_bytecode(bytecode *code, char flags) {
    // free the mementry elements
    mementry *memptr;
    size_t offset = code->elements;
    while(offset != 0) {
        memptr = *(mementry **)pop(code, &offset, sizeof(mementry *));
        _free_synth(memptr, flags);
    }

    // free the stack itself
    free_stack(code);
}

// TODO: does not work for multi-thread
table_level max_level;

// see protocol specifications in header file
void _recursiveconsume(bytecode *dst, stackptr ptr, stnode *subtree, char member_as_block) {
    switch(subtree->type){

        case NUMBER:
            // Push a reasonable representation of the value onto the stack
            *((number *)(_register(dst, ptr, NUMBER)->value = malloc(sizeof(number)))) 
                = DOUBLE(subtree->data.leaf.value);

            // ? might check for errors like overflow/underflow

            // free the text, since all the required data is now stored in number format
            free(subtree->data.leaf.value);

            break;

        case FIELD: {
            // just copy the mementry pointer
            mementry *fielddst = _register(dst, ptr, REFERENCE);
            fielddst->value = subtree->data.leaf.value;
            fielddst->level = ((mementry *)fielddst->value)->level;

            // reassign max_field
            if(fielddst->level > max_level)
                max_level = fielddst->level;
            
            break; }

        case STRING:
            // push pointer and don't free the orignial memory
            _register(dst, ptr, STRING)->value = subtree->data.leaf.value;

            // ? may reallocate space for string

            break;

        case BLOCK:
            // modify table scope
            _register(dst, ptr, EXPR)->value = _block;
            break;

        case BLOCK_END:
            // modify table scope
            _register(dst, ptr, EXPR)->value = _block_end;
            break;

        case EXPR:
            // process left child first
            _recursiveconsume(dst, ptr, subtree->data.parent.left, 0);

            // if there is one, process right child next
            if(subtree->data.parent.right != NULL)
                _recursiveconsume(dst, ptr, subtree->data.parent.right, 0);
            // make sure to provide a right-hand argument if it's a function call
            else if(subtree->data.parent.op->opcode == OP_CALL)
                _register(dst, ptr, UNDEFINED)->value = NULL;

            // then push operation onto the stack
            _register(dst, ptr, EXPR)->value = 
                implementationof(subtree->data.parent.op->opcode);
            break;

        case MEMBER: {
            stnode *tree;
            if(member_as_block){
                _register(dst, ptr, EXPR)->value = _block_end;
                while(subtree != NULL && subtree->data.parent.left != NULL) {
                    _recursiveconsume(dst, ptr, subtree->data.parent.left, 1);

                    // Reassign values for the loop
                    tree = subtree;
                    subtree = subtree->data.parent.right;

                    // Free the MEMBER node
                    free(tree);
                }
                _register(dst, ptr, EXPR)->value = _block;
                break;
            }
            // Create a new stack for the underlying function
            bytecode *function = create_stack(64);

            size_t offset = 0;

            // reset max level
            max_level = 0;

            // Consume a member chain into the stack
            while(subtree != NULL && subtree->data.parent.left != NULL) {
                // Consume the left subtree
                _recursiveconsume(function, (stackptr) &offset, subtree->data.parent.left, 1);

                // Reassign values for the loop
                tree = subtree;
                subtree = subtree->data.parent.right;

                // Free the MEMBER node
                free(tree);
            }

            // resize the stack to the minimal needed size
            fit_stack(function);

            // push function pointer to stack
            mementry *fundst = _register(dst, ptr, FUNCTION);
            fundst->value = function;
            // set functions maximally used level
            fundst->level = max_level;

            break;
        }

        case FILE_END:
            // Do not push anything
            break;
    }
    
    // free the subtree node
    free(subtree);
}

bytecode *consume(stnode *root) {
    
    // allocate the bytecode
    bytecode *code = create_stack(INITIAL_DATA_SIZE);

    // the stack offset
    size_t offset = 0;

    // post order traversal
    _recursiveconsume(code, (stackptr) &offset, root, 0);

    // return code
    return code;
}
