#include "parser.h"

#define DEFAULT_RBP 0
#define MAX_ERROR_LENGTH 512

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

    // TODO: Match closing brackets
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

// Function to create an error node
stnode *allocate_error(token *tkn, char *msg) {
    stnode *typed = allocate_typed(ERROR);
    typed->data.leaf.value = malloc(MAX_ERROR_LENGTH * sizeof(char));

    snprintf(typed->data.leaf.value, MAX_ERROR_LENGTH, 
        "[%li:%li] Parser error: %s '%s'", tkn->info.line, tkn->info.character, msg, tkn->content
    );
    
    return typed;
}

// Private declaration of some of the functions
stnode *expr(tkncache *cache, unsigned char op);
stnode *secondary(tkncache *cache);

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
            } else if(cache->cur->content[0] == '}') {
                // TODO: maybe return NULL
                ret = allocate_typed(BLOCK_END);
            } else {
                ret = allocate_error(cache->cur, "Unexpected bracket");
            }

            advance(cache);
            // TODO: handle other brackets

            return ret;
        }

        // End on EOF
        case NULLTKN:
            return allocate_typed(BLOCK_END);

        // Throw error when encountering invalid token
        case INVALID:
            stnode* ret = allocate_error(cache->cur, "Encountered invalid token");
            advance(cache);
            return ret;

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
    stnode *left = secondary(cache);

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
                return allocate_error(cache->cur, "Invalid operator");
            }
        }

    }

    // handle brackets
    if(cache->cur->type == BRACKET){

        // handle function calls
        if(cache->cur->content[0] == '(') {
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
                return allocate_error(cache->cur, "Expected closing bracket");
            }
        } else if (cache->cur->content[0] == '[') {
                        advance(cache);

            stnode *index = allocate_typed(INDEX);

            index->data.parent.left = left;
            left = index;

            // Set list of arguments to be expression
            index->data.parent.right = expr(cache, DEFAULT_RBP);

            // expect bracket
            if(cache->cur->type == BRACKET && cache->cur->content[0] == ']' ) {
                advance(cache);
            } else {
                return allocate_error(cache->cur, "Expected closing square bracket");
            }
        }

    }

    return left;
}

/*
    Decodes Fields/Values/Prefixes
    Returns a VALUE or one-handed EXPR or NULL
*/
stnode *secondary(tkncache *cache) {
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
                    return allocate_error(cache->cur, "Expected closing bracket");
                }

                return ret;
            } else if(cache->cur->content[0] == '{') {
                advance(cache);

                // BLOCK_END signals an in-expression block / function definition
                // TODO: maybe use another type name
                return allocate_typed(BLOCK_END);
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
                return allocate_error(cache->cur, "Invalid prefix operator");
            }
            
            // Throw an error if its not a prefix operator
            if(ret->data.parent.op->position != PREFIX) {
                return allocate_error(cache->cur, "Expected prefix operator");
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
        "Error", "Block", "BlockEnd", "Call", "Index", "Expr", "Value",
    };

    printf(". %s", typeNames[root->type]);
    // printf(". %i", root->type);

    if(root->type == EXPR)
        printf(" [%s]", root->data.parent.op->name);
    
    if(root->type == VALUE || root->type == ERROR) {
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