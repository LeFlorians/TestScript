#include "parser.h"

#define DEFAULT_RBP 0

/*
    Read a new token
    @param peek The token variable to overwrite
*/
void advance(tkncache *cache) {
    // Switch the two tokens and read a new one
    token *temp = cache->cur;
    cache->cur = cache->peek;
    cache->peek = temp;
    // Read next token, skip comments
    do {
        readtkn(cache->input, cache->peek);
    } while(cache->peek->type == COMMENT);
}

// Function to allocate stnode
stnode  *allocate_stnode() {
    // TODO: set default values
    return (stnode *)malloc(sizeof(stnode));
}

// Function to allocate stnode and set type
stnode  *allocate_typed(nodetype type) {
    stnode *node = (stnode *)malloc(sizeof(stnode));
    node->type = type;
    return node;
}

// Private declaration of some of the functions
stnode *phase1(tkncache *cache);
stnode *phase2(tkncache *cache);
stnode *expr(tkncache *cache, unsigned char op);

/*
    Parse function
    expect: 
        next token in cur
*/
stnode *parse(tkncache *cache) {
    switch (cache->cur->type) {
        case BRACKET: {
            stnode *ret;
            if(cache->cur->content[0] == '{') {
                // load member chain and return
                ret = allocate_typed(BLOCK);
            }
            if(cache->cur->content[0] == '}') {
                // TODO: maybe return NULL
                ret = allocate_typed(BLOCK_END);
            }

            advance(cache);
            // TODO: handle other brackets

            return ret;
        }

        // End on invalid tokens
        case INVALID:
        case NULLTKN:
            advance(cache);

            stnode *end = allocate_stnode();
            end->data.parent.left = NULL;
            end->data.parent.right = NULL;
            // TODO: return error if invalid
            end->type = BLOCK_END;
            return end;
        break;

    }

    // return an expression
    return expr(cache, DEFAULT_RBP);
}

/*
    Decodes any expression
    Returns an EXPR/VALUE node

    @param rbp Right binding power (default=OP_NONE)
    @param infeed Use this token instead of reading a new one
    @param pushback Used internally to return unused tokens to caller function
*/
stnode *expr(tkncache *cache, unsigned char rbp) {
    stnode *left = phase2(cache);

    if(left == NULL)
        return NULL;

    // act depending on peek's type
    switch(cache->cur->type) {

        case SYMBOL: {

            // allocate stack variables
            operator *op;

            stnode *new;

            while(cache->cur->type == SYMBOL 
                    && (op = mapop(cache->cur->content)) != 0 && op->precedence > rbp) {

                new = allocate_stnode();
                new->type = EXPR;
                
                // set operator and attach current 'left' to the new node
                new->data.parent.op = op;
                new->data.parent.left = left;

                advance(cache);
                
                // the right hand side will be a newly parsed expression,
                // taking associativity into consideration
                if(op->position == INFIX)
                    new->data.parent.right = expr(cache, op->precedence - op->associativity);
                else
                    new->data.parent.right = NULL;

                // set left to new
                left = new;
            }
            if(op == 0) {
                // TODO: Throw error, invalid operator in cache->cur->content
            }
        }

    }

    // handle function calls
    if(cache->cur->type == BRACKET && cache->cur->content[0] == '(') {
        advance(cache);

        stnode *call = allocate_typed(CALL);

        call->data.parent.left = left;
        left = call;

        // Set list of arguments to be expression
        call->data.parent.right = expr(cache, DEFAULT_RBP);

        // expect bracket
        if(cache->cur->type == BRACKET && cache->cur->content[0] == ')' ) {
            advance(cache);
        } else {
            // TODO: throw error, expected closing bracket
        }

    }

    return left;
}

/*
    Decodes Fields/Values/Prefixes
    Returns a VALUE or one-handed EXPR or NULL
*/
stnode *phase2(tkncache *cache) {
    stnode *ret, *val;

    // advance(cache);

    ret = allocate_stnode();
    val = ret;

    switch(cache->cur->type) {

        case BRACKET: {

            if(cache->cur->content[0] == '(') {
                // Advance, so the bracket gets unloaded
                advance(cache);

                // Return a newly parsed expression
                ret = expr(cache, DEFAULT_RBP);

                if(cache->cur->type == BRACKET && cache->cur->content[0] == ')') {
                    advance(cache);
                } else {
                    // TODO: throw error, expected closing bracket
                }

                return ret;
            }

            break;
        }

        case SYMBOL: {
            // Handle a symbol (only prefix operators allowed), then move on to setting value

            // set the operator
            operator op;

            // Map prefix operator
            ret->data.parent.op = mappreop(cache->cur->content);

            if(ret->data.parent.op == 0) {
                // TODO: throw error, invalid operator in cache->cur->content
            }
            
            // Throw an error if its not a prefix operator
            if(ret->data.parent.op->position != PREFIX) {
                // TODO: throw error
            }

            // set the node type
            ret->type = EXPR;

            // allocate new node for value and attach it to ret
            val = allocate_stnode();
            ret->data.parent.left = val;
            ret->data.parent.right = NULL;

            // read the next token for the value
            advance(cache);

            
            break;
        }

    }

    // TODO: handle other cases
    switch(cache->cur->type){
        case FIELD:
        case NUMBER:

            val->type = VALUE;

            // TODO: allocate and copy content properly
            
            int len = strlen(cache->cur->content) + 1;
            // allocate string
            char *dst = (char *) malloc(len * sizeof(char));
            strncpy(dst, cache->cur->content, len);

            val->data.leaf.value = dst;

            // Clean current value from cache
            advance(cache);

            return ret;

        break;
    }
    
    return allocate_typed(BLOCK_END);

}


// Function to generate a cache (reads one token from input already)
tkncache *gencache(FILE* input) {
    // create a token cache
    tkncache *cache = (tkncache *) malloc(sizeof(tkncache));

    cache->cur = (token *) malloc(sizeof(token));
    cache->peek = (token *) malloc(sizeof(token));
    cache->input = input;

    // advance once to load token into peek slot
    advance(cache);
    advance(cache);

    return cache;
}


/* Functions to print out syntaxtree */

void _printst(stnode *root, int depth) {
    // _printside(out, depth);

    static const char* typeNames[] = {
        "Block", "BlockEnd", "Call", "Expr", "Value",
    };

    printf(". %s", typeNames[root->type]);
    // printf(". %i", root->type);

    if(root->type == EXPR)
        printf(" [%s]", root->data.parent.op->name);
    
    if(root->type == VALUE) {
        printf(" (%s)", root->data.leaf.value);
    } else {

        if(root->data.parent.left != NULL){
            putchar('\n');
            for(int i = 0; i < depth; i++)
                printf("|");

            putchar('L');

            _printst(root->data.parent.left, depth+1);
        }

        if(root->data.parent.right != NULL){
            putchar('\n');
            for(int i = 0; i < depth; i++)
                printf("|");

            putchar('R');

            _printst(root->data.parent.right, depth+1);
        }

    }    
} 

void printst(stnode *root) {
    _printst(root, 0);
    putchar('\n');
}