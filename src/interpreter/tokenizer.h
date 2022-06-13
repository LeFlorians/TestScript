#include <stdio.h>

#include "../error/error.h"
#include "../memory/stack.h"

#define MAX_CONTENT 256

/*
* The Tokenizer takes a string and converts it to easy-to-understand tokens
*/

// Define all types
typedef enum {
    // shared
    FIELD,
    NUMBER,
    STRING,
    
    // token types
    NULLTKN, // Empty token (EOF, error)
    BRACKET,
    SYMBOL,

    // parser node types
    // TODO: replace most used with null
    FILE_END,   // no kids
    BLOCK,  // no kids
    BLOCK_END, // no kids
    MEMBER, 
    EXPR,   
} typing;

// Define a token
typedef struct {
    typing type;
    char *content;
} token;

// The cache contains everyting 'input' known to tokenizer & parser
typedef struct {
    FILE* input;
    token *cur;

    // for errors to be thrown by the tokenizer
    errorinfo info;

    // to keep track of how many characters were read
    unsigned int tokenpos;

    // bracket stack for parser
    stack *bracketstack;
} cache;

// Read next token from stream and place it in dst
void readtkn(cache *cache);

