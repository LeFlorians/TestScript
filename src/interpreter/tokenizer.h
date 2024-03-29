#pragma once

#include <stdio.h>

#include "error/error.h"
#include "memory/stack.h"

#define MAX_CONTENT 1024 

/*
* The Tokenizer takes a string and converts it to easy-to-understand tokens
*/

// Define all types
typedef enum {
    // shared
    NUMBER,
    STRING,

    // runtime types
    UNDEFINED,
    ARRAY, TUPLE, // both arrays, but internal differences
    OBJECT, // an object is a block of data
    FUNCTION, CFUNCTION,
    REFERENCE, // a reference is what the bytecode compiler creates from a field

    // token types
    NULLTKN, // Empty token (EOF, error)
    BRACKET,
    SYMBOL,
    FIELD,

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
    // the corresponding stack offset
    size_t bracketstack_offset;

    // tokenizer flag for parser to check if a space was encountered during readtkn()
    char space;
} cache;

// Read next token from stream and place it in dst
void readtkn(cache *cache);

