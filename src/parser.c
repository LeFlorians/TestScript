#include "parser.h"

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

// Private declaration of some of the functions
stnode *phase0(tkncache *cache);
stnode *phase1(tkncache *cache);
stnode *phase2(tkncache *cache);
stnode *expr(tkncache *cache, unsigned char op);


/*
    Builds chain of phase1 members
    Stops when reaching BLOCK_END
    Returns a MEMBER node
    -> left: block/expr, right: member/block_end

    This is used to parse an entire file into a parse tree
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
    } while(cur->data.parent.right->type != BLOCK_END);

    // Don't return the empty BLOCK_END node
    // return cur->data.parent.left;
    return cur;
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
            ret->data.parent.left = NULL;
            ret->data.parent.right = NULL;

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

        // End on invalid tokens
        case INVALID:
        case NULLTKN:
            stnode *end = allocate_stnode();
            end->data.parent.left = NULL;
            end->data.parent.right = NULL;
            end->type = BLOCK_END;
            return end;
        break;

    }

    // return an expression
    return expr(cache, 0xFF);
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

            // advance, then act on cur
            advance(cache);

            // allocate stack variables
            operator *op;

            stnode *new;

            while(cache->cur->type == SYMBOL 
                    && (op = mapop(cache->cur->content))->precedence < rbp) {

                new = allocate_stnode();
                new->type = EXPR;
                
                // set operator and attach current 'left' to the new node
                new->data.parent.op = op;
                new->data.parent.left = left;

                advance(cache);
                
                // the right hand side will be a newly parsed expression,
                // taking associativity into consideration
                if(op->position == INFIX)
                    new->data.parent.right = expr(cache, op->precedence + op->associativity);
                else
                    new->data.parent.right = NULL;

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

    // advance(cache);

    ret = allocate_stnode();
    val = ret;


    if(cache->cur->type == SYMBOL) {
        // set the operator
        operator op;

        // Map prefix operator
        ret->data.parent.op = mappreop(cache->cur->content);
        
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
    }
    
    val->type = VALUE;

    // TODO: allocate and copy content properly
    
    int len = strlen(cache->cur->content) + 1;
    // allocate string
    char *dst = (char *) malloc(len * sizeof(char));
    strncpy(dst, cache->cur->content, len);

    val->data.leaf.value = dst;

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

    // advance once to load token into peek slot
    advance(&cache);

    return phase0(&cache);
}


/* Functions to print out syntaxtree */

void _printside(int depth) {
    // Print lines on the side
    for(int i = 0; i < depth; i++)
        printf("|");
}

void _printst(stnode *root, int depth) {
    // _printside(out, depth);

    static const char* typeNames[] = {
        "BlockEnd", "Member", "FuncEnd", "Expr", "Value",
    };

    printf(". %s", typeNames[root->type]);
    // printf(". %i", root->type);

    if(root->type == EXPR)
        printf(" [%s]", root->data.parent.op->name);
    
    if(root->type == VALUE) {
        printf(" (%s)", root->data.leaf.value);
    } else {
        putchar('\n');

        if(root->data.parent.left != NULL){
            _printside(depth);

            putchar('L');

            _printst(root->data.parent.left, depth+1);

            putchar('\n');
        }

        if(root->data.parent.right != NULL){
            _printside(depth);

            putchar('R');

            _printst(root->data.parent.right, depth+1);
        }
        

    }

    
} 

void printst(stnode *root) {
    _printst(root, 0);
}