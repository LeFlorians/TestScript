#include "localizer.h"

void _localize_member(stnode *member, hashtable *table, errorinfo *info) {
    // localize child first
    if(member->data.parent.right != NULL)
        _localize_member(member->data.parent.right, table, info);

    // localize current expression
    localize(member->data.parent.left, table, info);
}

void localize(stnode *subtree, hashtable *table, errorinfo *info) {

    // Resolve fields
    switch(subtree->type) {
        case EXPR: {
            // Go in order
            if(subtree->data.parent.op->position == INFIX) {
                if(subtree->data.parent.op->associativity == LTR) {
                    localize(subtree->data.parent.left, table, info);
                        if(subtree->data.parent.right != NULL)
                    localize(subtree->data.parent.right, table, info);
                } else {
                    if(subtree->data.parent.right != NULL)
                        localize(subtree->data.parent.right, table, info);
                    localize(subtree->data.parent.left, table, info);
                }
            } else 
                localize(subtree->data.parent.left, table, info);
            break;
        }

        case MEMBER:

            // localize child first
            if(subtree->data.parent.right != NULL)
                _localize_member(subtree->data.parent.right, table, info);

            // localize current expression
            localize(subtree->data.parent.left, table, info);

            break;

        case BLOCK:
            // adjust table
            ht_up(table);
            break;

        case BLOCK_END:
            // adjust table
            ht_down(table);
            break;

        case FIELD: {
            // resolve field into reference
            char *key = subtree->data.leaf.value;
            subtree->data.leaf.value = find(table, key);

            // free the key
            free(key);

            break; }

        default:
            // Ignore
            break;
    }

}
