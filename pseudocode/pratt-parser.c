stnode* parse() {
    return expr(0); // parse is a wrapper function for expr
}

stnode *expr(int precedence) {
    stnode *left = secondary(); // Generate a secondary node
    stnode *ret = left;

    if(left == NULL) return NULL; // Reached the end of the source code

    token next;
    while(precedence_of(next = peek_token()) > precedence 
            && next.type == SYMBOL) {
        ret = allocate_stnode(EXPR); // Allocate a new node
        ret->left_child = left;
        ret->right_child = expr(presedence_of(next) - associativity_of(next));
        ret->operator = operator_of_token(next);

        next = advance(); // Throw away the token, request a new one
        left = ret;
    }
    return ret;
}

stnode *secondary() {
    return stnode_from(advance());  // See the full source code for details
}
