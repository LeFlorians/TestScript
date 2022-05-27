#include "parser.h"

// helper function to allocate an stnode
stnode *allocate_stnode() {
    return (stnode *)malloc(sizeof(stnode));
}

// allocate an stnode with given type
stnode *allocate_typed(nodetype type) {
    stnode *node = (stnode *)malloc(sizeof(stnode));
    node->type = type;
    return node;
}

// Read next token into cache and match brackets
void advance(cache *cache) {
    readtkn(cache);

    if(cache->cur->type == BRACKET){

        static char closing[] = {'}', ')', ']'};
        switch(cache->cur->content[0]) {
            case '{':
                push(cache->bracketstack, &closing[0]);
                break;
            case '(':
                push(cache->bracketstack, &closing[1]);
                break;
            case '[':
                push(cache->bracketstack, &closing[2]);
                break;

            default:
                char *popped;

                if((popped = (char *)pop(cache->bracketstack)) == NULL) {
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

// Define the following functions
stnode *expr(cache *cache, unsigned char rbp);
stnode *secondary(cache *cache);


// Builds a member tree of multiple expressions
stnode *subexpr(cache *cache, unsigned char rbp) {
    stnode *subroot, *member;
    
    subroot = member = expr(cache, rbp);

    // Parse in-expression code blocks
    if(member->type == BLOCK){
        // loop variable
        stnode *left;
        
        // change type from block to member
        member->type = MEMBER;

        // keep track of level
        unsigned int level = 1;

        // left child is expression or BLOCK_END
        // right child is member pointer or NULL

        char flag = 0;

        while(1) {
            left = parse(cache);

            // pass on errors / EOF and ignore sub-scope
            if(left->type == FILE_END)
                return left;

            // keep track of level
            else if(left->type == BLOCK)
                level++;
            else if(left->type == BLOCK_END){
                level--;
                if(level == 0) 
                    break;
            }

            // skip this once
            if(flag) {                            
                member = member->data.parent.right = allocate_typed(MEMBER);
            } else
                flag = 1;
            
            member->data.parent.left = left;
            
        }

        // Set left pointer to NULL
        member->data.parent.right = NULL;
    }
    return subroot;
}

// Parses a single expression
stnode *expr(cache *cache, unsigned char rbp) {
    stnode *left = secondary(cache);

    // If secondary is done, just return
    if(left->type == FILE_END)
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
                new->data.parent.right = subexpr(cache, op->precedence - op->associativity);
            } else {
                new->data.parent.right = NULL;

                // return on semicolon
                if(op->opcode == OP_END)
                    return left;
            }

            left = new;
        }
        if(op == 0) {
            // TODO: free already allocated nodes

            throw(EP_INVALID_OPERATOR, &cache->info);
            return allocate_typed(FILE_END);
        }
    }

    // Handle function calls and indexing
    if(cache->cur->type == BRACKET) {
        switch(cache->cur->content[0]) {

            case '(': {
                advance(cache);

                stnode *call = allocate_typed(CALL);

                call->data.parent.left = left;
                left = call;

                // Set list of arguments to be expression
                call->data.parent.right = subexpr(cache, 0);

                // expect closing bracket
                if(cache->cur->type == BRACKET && cache->cur->content[0] == ')')
                    advance(cache);
                else {
                    // TODO: free already allocated nodes

                    throw(EP_EXPECTED_CLOSING_BRACKET, &cache->info);
                    return allocate_typed(FILE_END);
                }

                goto repeat;
            }

            case '[': {
                advance(cache);

                stnode *index = allocate_typed(INDEX);

                index->data.parent.left = left;
                left = index;

                // Set list of arguments to be expression
                index->data.parent.right = subexpr(cache, 0);

                // expect closing bracket
                if(cache->cur->type == BRACKET && cache->cur->content[0] == ']')
                    advance(cache);
                else {
                    // TODO: free already allocated nodes

                    throw(EP_EXPECTED_CLOSING_SQUARE_BRACKET, &cache->info);
                    return allocate_typed(FILE_END);                
                }

                goto repeat;
            }

        }
    }

    return left;
}

// Parses a single 'secondary' value
// Returns FILE_END on error
stnode *secondary(cache *cache){
    // declare node to return
    stnode *ret = NULL;

    switch(cache->cur->type) {
        case BRACKET:
            switch(cache->cur->content[0]) {
                case '(':
                    // advance, so the bracket gets unloaded
                    advance(cache);

                    // parse subexpression
                    ret = subexpr(cache, 0);

                    // expect closing bracket
                    if(cache->cur->type == BRACKET && cache->cur->content[0] == ')'){
                        advance(cache);
                        return ret;
                    }
                    // TODO: free ret

                    throw(EP_EXPECTED_CLOSING_BRACKET, &cache->info);
                    return allocate_typed(FILE_END);

                case '{':
                    advance(cache);
                    return allocate_typed(BLOCK);

                case '}':
                    advance(cache);
                    return allocate_typed(BLOCK_END);
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

            break;

        // Return on nulltkn
        case NULLTKN:
            return allocate_typed(FILE_END);
    }

    // ? match strings too

    if(cache->cur->type == FIELD || cache->cur->type == NUMBER || cache->cur->type == STRING){
        stnode *val;

        // TODO: safe the type of the value as well
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
    return expr(cache, 0);
}
