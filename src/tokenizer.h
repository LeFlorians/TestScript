#include <stdio.h>

#define MAX_CONTENT 256
#define MAX_CONTENTSTR "256"

/*
* The Tokenizer takes a string and converts it to easy-to-understand tokens
*/

// Define token types

typedef enum {
    NULLTKN, // Empty token
    INVALID, // content always carries error message
    BRACKET,
    FIELD,
    SYMBOL,
    STRING,
    NUMBER,
} tkntype;

// Debug info struct
typedef struct {
    unsigned int line, character;
} debuginfo;

// Define a token
typedef struct {
    tkntype type;
    char content[MAX_CONTENT];
} token;

typedef struct {
    FILE* input;
    token *cur;
    debuginfo info;
} tokencache;


// Read next token from stream and place it in dst
void readtkn(tokencache *cache);
