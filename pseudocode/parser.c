// implement functions
stnode* parse() {
    // just call expr
    return expr(0);
}

stnode *expr(int precedence) {
    // read secondary
    stnode *left = secondary();
    stnode *ret = left;

    // reached end of file
    if(left == NULL)
        return NULL;

    token next;
    while(precedence_of(next = peek_token()) > precedence 
            && next.type == SYMBOL) {
        // allocate a new node
        ret = allocate_stnode(EXPR);
        ret->left_child = sec;
        ret->right_child = expr(presedence_of(next) - associativity_of(next));
        ret->operator = operator_of_token(next);

        // throw away token
        next = next_token();

        sec = ret;
    }
    return ret;
}

stnode *secondary() {
    // dealing with brackets left out, see full source code...
    return stnode_from(next_token()); 
}
