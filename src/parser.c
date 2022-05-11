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

// TODO : readtkn only return useful tokens

/*
    Builds chain of phase1 members
    Stops when reaching BLOCK_END
    Returns a MEMBER node
    -> left: block/expr, right: member/block_end
*/
stnode *phase0(FILE* input) {
    stnode *start, *next, *cur;
   
    // allocate starting node
    start = allocate_stnode();
    start->type = MEMBER;

    cur = start;

    while(cur->type != BLOCK_END){
        // set left to be block/expr
        cur->left = phase1(input);

        next = allocate_stnode();
        next->type = cur->left->type == BLOCK_END ? BLOCK_END : MEMBER;
        
        cur->right = next;
        cur = next;

    }
    return start;
}

/*
    Returns EXPR/BLOCK nodes
    -> left: MEMBER
*/
stnode *phase1(FILE* input) {
    stnode *ret = allocate_stnode();
    token tkn;

    readtkn(input, &tkn);

    switch (tkn.type) {
        case BRACKET: {
            if(tkn.content[0] == '{') {
                // set type, load member chain into block, return
                ret->type = BLOCK;
                ret->left = phase0(input);
                return ret;
            }
            if(tkn.content[0] == '}') {
                // signal block end and return
                ret->type = BLOCK_END;
                return ret;
            }
            if(tkn.content[0] == ')') {
                // signal block end and return
                ret->type = FUNC_END;
                return ret;
            }
            // TODO: wrong bracket / undefined 
            break;
        }
        case FIELD: {
            // Create expression
            ret->type = EXPR;

            // deal with field
            phase2(input, &ret);

            break;
        }
        case SYMBOL: {
            // prefix operators
            ret->type = EXPR;

            // TODO: improve
            ret->op = mapop(tkn.content);

            // ret->left will be the next field
            readtkn(input, &tkn);
            if(tkn.type != FIELD){
                // TODO: Throw error. need field after prefix operator
            }
            ret->left = allocate_stnode();
            // TODO: load contents & info from field

            break;
        }
    }

    return ret;
}

/*
    Decodes what comes after a field
    Acts on expression
*/
stnode *phase2(FILE* input, stnode *expr) {
    stnode *ret = allocate_stnode();
    token tkn;

    readtkn(input, &tkn);

    switch (tkn.type) {
        case BRACKET: {
            if(tkn.content[0] == '(') {
                // function call, since ( after field
                // TODO: call function
                ret->type = FUNC;
                ret->left = phase1(input);

            }
            // wrong bracket

            break;
        }
        case SYMBOL: {
            // deal with postfix/infix operators
            expr->op = mapop(tkn.content);

            if(ispostfix(ret->op)) {

            }
            if(isinfix(ret->op)) {
                
            }

        }
    }

    return ret;
}


/*
    Decodes what comes after an infix operator (SYMBOL)
*/
stnode *phase2(FILE* input, stnode *expr) {

}

// rbp=right binding power
void expr(FILE *input, char rbp) {
    token tkn;

    // Load next token
    readtkn(input, &tkn);

    stnode *left = nulldenotation(tkn.content);
    

}

void parse(FILE* input, stnode *dst) {
    // Allocate some stack variables
    token tkn;

    // Get the next token
    readtkn(input, &tkn);




}