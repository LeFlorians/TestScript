#include "parser.h"
#include "memory/hashtable.h"

// Takes a syntax tree and resolves fields
void localize(stnode *tree, hashtable *mem, errorinfo *info);