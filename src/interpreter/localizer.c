#include "localizer.h"

void _localize_member(stnode *member, hashtable *table) {
    // localize child first
        if(member->data.parent.right != NULL)
            _localize_member(member->data.parent.right, table);

    // localize current expression
        localize(member->data.parent.left, table);
}

void localize(stnode *subtree, hashtable *table) {

    // Resolve fields
    switch(subtree->type) {
        case EXPR: {
            // Go in order
            if(subtree->data.parent.op->position == INFIX) {
                if(subtree->data.parent.op->associativity == LTR) {
                    localize(subtree->data.parent.left, table);
                        if(subtree->data.parent.right != NULL)
                    localize(subtree->data.parent.right, table);
                } else {
                    if(subtree->data.parent.right != NULL)
                        localize(subtree->data.parent.right, table);
                    localize(subtree->data.parent.left, table);
                }
            } else 
                localize(subtree->data.parent.left, table);
            break;
        }

        case MEMBER:
            // change table scope
            ht_up(table);

            // localize child first
            if(subtree->data.parent.right != NULL)
                _localize_member(subtree->data.parent.right, table);

            // localize current expression
            localize(subtree->data.parent.left, table);

            // reset table scope
            ht_down(table);

            break;

        case BLOCK:
            // adjust table
            ht_up(table);
            break;

        case BLOCK_END:
            // adjust table
            ht_down(table);
            break;

        case FIELD:
            // resolve field into reference
            char *key = subtree->data.leaf.value;
            subtree->data.leaf.value = find(table, key);

            // free the key
            free(key);

            break;

        default:
            // Ignore
            break;
    }

}