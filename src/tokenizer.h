#include <stdio.h>

#define MAX_CONTENT 256
#define MAX_CONTENTSTR "256"

/*
* The Tokenizer takes a string and converts it to easy-to-understand tokens
*/

// Define token types

typedef enum {
    INVALID, // content always carries error message
    NULLTKN, // Empty token
    COMMENT,
    BRACKET,
    FIELD,
    SYMBOL,
    STRING,
    NUMBER,
} tkntype;

// Define a token
typedef struct {
    tkntype type;
    char content[MAX_CONTENT];
    struct {
        unsigned long character, line;
    } info;
} token;

// Read next token from stream and place it in dst
void readtkn(FILE *input, token *dst);
