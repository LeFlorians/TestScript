/*
* The Tokenizer takes a string and converts it to easy-to-understand tokens
*/

// Define a token
typedef struct token token;

struct token {
    enum {
        STRING, NUMBER,
    } type;
    char content[256];
};

// Read one token from stream and place it in dst
void tokenize(FILE *input, token *dst);
