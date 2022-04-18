/*
* The Tokenizer takes a string and converts it to easy-to-understand tokens
*/

// Define a token
typedef struct token token;

struct token {
    enum {
        INVALID, // content always carries error message
        NULLTKN, // Empty token
        COMMENT,
        BRACKET,
        FIELD,
        SYMBOL,
        STRING,
        INTEGER,
        NUMBER, // floating-point
    } type;
    char content[256];
    union {
        unsigned long long integer; // only if INTEGER
        double number; // only if NUMBER
    } num;
};

// Read next token from stream and place it in dst
void readtkn(FILE *input, token *dst);
