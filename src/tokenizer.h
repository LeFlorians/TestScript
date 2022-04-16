/*
* The Tokenizer takes a string and converts it to easy-to-understand tokens
*/

// Define a token
typedef struct token token;

struct token {
    enum {
        INVALID,
        BRACKET,
        FIELD,
        STRING,
        INTEGER,
        NUMBER, // floating-point
    } type;
    char content[256];
    union {
        long long integer; // only if INTEGER
        double number; // only if NUMBER
    } num;
};

// Read one token from stream and place it in dst
void tokenize(FILE *input, token *dst);
