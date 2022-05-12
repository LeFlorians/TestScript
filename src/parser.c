#include "parser.h"

// Function to map a char *operator to an operator type
// TODO: binary search
operator mapop(char* op) {
    
}

// TODO: implement
char isprefix(operator op) {

}

char isinfix(operator op) {

}

char ispostfix(operator op) {

}

// For simplification
typedef struct {
    token *cur, *peek;
    FILE *input;
} tkncache;

/*
    Read a new token
    @param peek The token variable to overwrite
*/
void advance(tkncache *cache) {
    // Switch the two tokens and read a new one
    token *temp = cache->cur;
    cache->cur = cache->peek;
    cache->peek = temp;
    // TODO: readtkn should only return useful tokens
    readtkn(cache->input, cache->peek);
}

// Function to allocate stnode
stnode  *allocate_stnode() {
    // TODO: set default values
    return (stnode *)malloc(sizeof(stnode));
}

// Private declaration of some of the functions
stnode *phase0(tkncache *cache);
stnode *phase1(tkncache *cache);
stnode *phase2(tkncache *cache);
stnode *expr(tkncache *cache, operator op);

/*
    Builds chain of phase1 members
    Stops when reaching BLOCK_END
    Returns a MEMBER node
    -> left: block/expr, right: member/block_end
*/
stnode *phase0(tkncache *cache) {

    stnode *cur, *last = NULL;

    do {
        cur = allocate_stnode();
        cur->type = MEMBER;

        // set right to be expression / block
        cur->data.parent.right = phase1(cache);

        // Append current node to next node (reverse direction / bottom-up)  
        cur->data.parent.left = last;
        last = cur;
    } while(cur->type != BLOCK_END);

    // Don't return the empty BLOCK_END node
    return cur->data.parent.left;
}

/*
    Returns EXPR nodes
    -> left: MEMBER
*/
stnode *phase1(tkncache *cache) {

    // read next token
    advance(cache);

    switch (cache->cur->type) {
        case BRACKET: {
            if(cache->cur->content[0] == '{') {
                // load member chain and return
                return phase0(cache);
            }

            // allocate a return node
            stnode *ret = allocate_stnode();

            if(cache->cur->content[0] == '}') {
                // signal block end and return
                ret->type = BLOCK_END;
                return ret;
            }
            if(cache->cur->content[0] == ')') {
                // signal function end and return
                ret->type = FUNC_END;
                return ret;
            }
            // TODO: wrong bracket / undefined 
            break;
        }

    }

    // create stack variable of token address as pushback placeholder
    token *addr = NULL;

    // return  an expression
    return expr(cache, OP_NULL);
}

/*
    Decodes any expression
    Returns an EXPR/VALUE node

    @param rbp Right binding power (default=OP_NONE)
    @param infeed Use this token instead of reading a new one
    @param pushback Used internally to return unused tokens to caller function
*/
stnode *expr(tkncache *cache, operator rbp) {
    stnode *left = phase2(cache);

    // act depending on peek's type
    switch(cache->peek->type) {
        case BRACKET: {

            // advance
            advance(cache);

            if(cache->cur->content[0] == '(') {
                // TODO: this would be a function call

            }
            if(cache->cur->content[0] == ')') {
                // TODO: maybe return sth else
                return left;
            }

            break;
        }
        case SYMBOL: {

            // allocate stack variables
            operator op ;
            stnode *new;

            while((op = mapop(cache->peek->content)) >= rbp 
                    && cache->peek->type == SYMBOL) {

                new = allocate_stnode();
                
                // set operator and attach current 'left' to the new node
                new->data.parent.op = op;
                new->data.parent.left = left;

                advance(cache);
                
                // the right hand side will be a newly parsed expression
                if(isinfix(op))
                    new->data.parent.right = expr(cache, op);
                

                // updat operator
                op = mapop(cache->peek->content);

                // set left to new
                left = new;
            }
        }

    }

    return left;
}

/*
    Decodes Fields/Values/Prefixes
    Returns a VALUE or one-handed EXPR
*/
stnode *phase2(tkncache *cache) {
    stnode *ret, *val;

    advance(cache);

    ret = allocate_stnode();
    val = ret;


    if(cache->cur->type == SYMBOL) {

        // set the operator
        ret->data.parent.op = mapop(cache->cur->content);
        
        // Throw an error if its not a prefix operator
        if(!isprefix(ret->data.parent.op)) {
            // TODO: throw error
        }

        // set the node type
        ret->type = EXPR;

        // allocate new node for value and attach it to ret
        val = allocate_stnode();
        ret->data.parent.left = val;

        // read the next token for the value
        advance(cache);
    }
    
    val->type = VALUE;

    // TODO: allocate and copy content properly
    val->data.leaf.value = cache->cur->content;

    return ret;
}


stnode *parse(FILE* input) {
    // allocate current & peek tokens
    token cur, peek;
    
    // create a token cache
    tkncache cache;
    cache.cur = &cur;
    cache.peek = &peek;
    cache.input = input;

    return phase0(&cache);
}