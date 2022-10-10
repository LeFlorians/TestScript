#include "parser.h"

// helper function to allocate an stnode
static inline stnode *allocate_stnode() {
    return (stnode *)malloc(sizeof(stnode));
}

// allocate an stnode with given type
static inline stnode *allocate_typed(nodetype type) {
    stnode *node = (stnode *)malloc(sizeof(stnode));
    node->type = type;
    return node;
}

// helper function to free stnode of any time
void free_stnode(stnode *node) {
    switch(node->type) {
        case MEMBER:
        case EXPR:
            // free child nodes
            if(node->data.parent.left != NULL)
                free_stnode(node->data.parent.left);
            if(node->data.parent.right != NULL)
                free_stnode(node->data.parent.right);
            break;
        
        // these have no properties to be freed
        case BLOCK:
        case BLOCK_END:
        case FILE_END:
            break;

        // the rest are value-based nodes
        default:
            // free the values
            free(node->data.leaf.value);
            break;
    }
    // free the node itsself
    free(node);
}

// Read next token into cache and match brackets
static inline void advance(cache *cache) {
    readtkn(cache);

    if(cache->cur->type == BRACKET){

        switch(cache->cur->content[0]) {
            case '{':
                *((char *) push(cache->bracketstack, &cache->bracketstack_offset, 1)) = '}';
                break;
            case '(':
                *((char *) push(cache->bracketstack, &cache->bracketstack_offset, 1)) = ')';
                break;
            case '[':
                *((char *) push(cache->bracketstack, &cache->bracketstack_offset, 1)) = ']';
                break;

            default: {
                char *popped;

                if((popped = (char *)pop(cache->bracketstack,
                                &cache->bracketstack_offset, 1)) == NULL) {
                    // Throw error, no closing bracket expected
                    throw(EP_INVALID_CLOSING_BRACKET, &cache->info);

                    cache->cur->type = FILE_END;
                    break;
                }

                if(*popped != cache->cur->content[0] ){
                    // Throw error, wrong bracket
                    throw(EP_INVALID_CLOSING_BRACKET, &cache->info);

                    cache->cur->type = FILE_END;
                    break;
                }
            break;
            }
        }
    }
}

// function to initialize the parser
void parser_init(cache *cache) {
    // advance the cache once
    advance(cache);
}

// Define the following functions
stnode *expr(cache *cache, unsigned char rbp);
stnode *secondary(cache *cache, unsigned char rbp);

// parser-defined operators
operator virtualops[] = {
    {"f( )", OP_CALL, 0, 0, 0},     // virtual call operator
    {"g[ ]", OP_INDEX, 0, 0, 0},    // virtual index operator
    {"[ ]", OP_ARRAY, 0, 0, 0},    // virtual index operator
};

// Parses a single expression
char in_expression; // does not work for multi-thread
stnode *expr(cache *cache, unsigned char rbp) {

    stnode *left = secondary(cache, rbp);
    in_expression = 1;

    // If secondary is done, just return
    if(left == NULL || left->type == FILE_END)
        return left;

    // label to jump to after function call / indexing
    repeat:

    if(cache->cur->type == SYMBOL) {
        operator *op;
        stnode *new;

        while(
            cache->cur->type == SYMBOL &&
            (op = mapop(cache->cur->content)) != 0 &&
            op->precedence > rbp
        ) {
            new = allocate_typed(EXPR);

            new->data.parent.op = op;
            new->data.parent.left = left;

            advance(cache);

            if(op->position == INFIX){
                new->data.parent.right = expr(cache, op->precedence - op->associativity);
            } else {
                new->data.parent.right = NULL;

                // return on semicolon
                if(op->opcode == OP_END)
                    return left;
            }

            left = new;
        }
        if(op == 0) {
            // free already allocated nodes
            free_stnode(left);
            
            throw(EP_INVALID_OPERATOR, &cache->info);
            return allocate_typed(FILE_END);
        }
    }

    // Handle function calls and indexing
    //  but only if there was no space in between the brackets and symbol
    if(cache->cur->type == BRACKET && !cache->space) {
        switch(cache->cur->content[0]) {

            case '(':
                advance(cache);

                stnode *call = allocate_typed(EXPR);
                call->data.parent.op = &virtualops[0];

                call->data.parent.left = left;
                left = call;

                // Set list of arguments to be expression
                call->data.parent.right = expr(cache, 0);

                // expect closing bracket
                if(cache->cur->type == BRACKET && cache->cur->content[0] == ')')
                    advance(cache);
                else {
                    // free already allocated nodes
                    free_stnode(left);

                    throw(EP_EXPECTED_CLOSING_BRACKET, &cache->info);
                    return allocate_typed(FILE_END);
                }

                goto repeat;

            case '[':
                advance(cache);

                stnode *index = allocate_typed(EXPR);
                index->data.parent.op = &virtualops[1];

                index->data.parent.left = left;
                left = index;

                // Set list of arguments to be expression
                index->data.parent.right = expr(cache, 0);

                // expect closing bracket
                if(cache->cur->type == BRACKET && cache->cur->content[0] == ']')
                    advance(cache);
                else {
                    // free already allocated nodes
                    free_stnode(left);

                    throw(EP_EXPECTED_CLOSING_SQUARE_BRACKET, &cache->info);
                    return allocate_typed(FILE_END);                
                }

                goto repeat;
        }
    }

    return left;
}

// Parses a single 'secondary' value
// Returns FILE_END on error
stnode *secondary(cache *cache, unsigned char rbp){
    // declare node to return
    stnode *ret = NULL;

    switch(cache->cur->type) {
        case BRACKET:
            switch(cache->cur->content[0]) {
                case '(':
                    // advance, so the bracket gets unloaded
                    advance(cache);

                    // parse subexpression
                    ret = expr(cache, 0);

                    // expect closing bracket
                    if(cache->cur->type == BRACKET && cache->cur->content[0] == ')'){
                        advance(cache);
                        return ret;
                    }

                    // free ret
                    free_stnode(ret);

                    throw(EP_EXPECTED_CLOSING_BRACKET, &cache->info);
                    return allocate_typed(FILE_END);

                case '{': {
                    advance(cache);

                    // when this is a stand-alone bracket, just return a BLOCK
                    if(!in_expression)
                        return allocate_typed(BLOCK);

                    ret = allocate_typed(MEMBER);
                    
                    // loop variable
                    stnode *left, *last;

                    // Set right and left pointers to NULL
                    ret->data.parent.right = NULL;
                    ret->data.parent.left = NULL;

                    // keep track of level
                    unsigned int level = 1;

                    // left child is expression or BLOCK_END
                    // right child is member pointer or NULL

                    char flag = 0;

                    while(1) {
                        // next token will close
                        if(cache->cur->type == BRACKET &&
                                cache->cur->content[0] == '}') {
                            advance(cache);
                            if(--level == 0)
                                break;
                        }

                        left = expr(cache, 0);

                        if(left == NULL)
                            break;

                        // pass on errors / EOF and ignore sub-scope
                        if(left->type == FILE_END)
                            return left;

                        // keep track of level
                        else if(left->type == BLOCK)
                            level++;

                        // skip this once
                        if(flag) {    
                            // reverse direction
                            last = ret;
                            ret = allocate_typed(MEMBER);
                            ret->data.parent.right = last;                  
                        } else
                            flag = 1;

                        ret->data.parent.left = left;
                    }

                    return ret; }

                case '}':
                    advance(cache);
                    return allocate_typed(BLOCK_END);

                // array creation
                case '[':
                    advance(cache);

                    ret = allocate_typed(EXPR);
                    ret->data.parent.op = &virtualops[2]; // array operator

                    // set list of arguments to be expression
                    ret->data.parent.left = expr(cache, 0);

                    // expect closing bracket
                    if(cache->cur->type == BRACKET && cache->cur->content[0] == ']')
                        advance(cache);
                    else {
                        // free already allocated nodes
                        free_stnode(ret);

                        throw(EP_EXPECTED_CLOSING_SQUARE_BRACKET, &cache->info);
                        return allocate_typed(FILE_END);                
                    }
                    return ret;

                // End function calls / indexing
                case ')':
                case ']':
                    return NULL;
            }
            break;

        case SYMBOL:        
            // allocate return node
            ret = allocate_typed(EXPR);

            // map prefix operator
            ret->data.parent.op = mappreop(cache->cur->content);

            if(ret->data.parent.op == 0){
                throw(EP_INVALID_OPERATOR, &cache->info);

                ret->type = FILE_END;
                return ret;
            }

            if(ret->data.parent.op->position != PREFIX) {
                throw(EP_EXPECTED_PREFIX_OPERATOR, &cache->info);
                
                ret->type = FILE_END;
                return ret;
            }
            // advance to unload operator
            advance(cache);

            break;

        // Return on nulltkn
        case NULLTKN:
            return allocate_typed(FILE_END);
    }

    if(cache->cur->type == FIELD || cache->cur->type == NUMBER || cache->cur->type == STRING){
        stnode *val;

        if(ret == NULL) {
            val = (ret = allocate_typed(cache->cur->type));
        } else {
            ret->data.parent.left = (val = allocate_typed(cache->cur->type));
        }

        // copy and resize content
        size_t len = strlen(val->data.leaf.value = cache->cur->content);
        if(len < MAX_CONTENT)
            val->data.leaf.value = realloc(val->data.leaf.value, len + 1);

        // allocate new space for another token to be read
        cache->cur->content = malloc(MAX_CONTENT);

        advance(cache);

        return ret;
    }

    throw(EP_TOKEN_UNMATCHED, &cache->info);

    if(ret == NULL)
        ret = allocate_stnode();
    
    ret->type = FILE_END;
    return ret;
}

stnode *parse(cache *cache){
    in_expression = 0;
    return expr(cache, 0);
}
