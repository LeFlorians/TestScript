#include <stdio.h>

#include "../error/error.h"
#include "../memory/stack.h"

#define MAX_CONTENT 256

/*
* The Tokenizer takes a string and converts it to easy-to-understand tokens
*/

// Define token types

typedef enum {
    NULLTKN, // Empty token (EOF, error)
    BRACKET,
    FIELD,
    SYMBOL,
    STRING,
    NUMBER,
} tkntype;

// Define a token
typedef struct {
    tkntype type;
    char *content;
} token;

// The cache contains everyting 'input' known to tokenizer & parser
typedef struct {
    FILE* input;
    token *cur;

    // for errors to be thrown by the tokenizer
    errorinfo info;

    // bracket stack for parser
    stack *bracketstack;
} cache;

// Read next token from stream and place it in dst
void readtkn(cache *cache);

