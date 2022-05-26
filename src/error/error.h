// Enum of all messages
enum errorformat {
    // Tokenizer errors
    ET_QUOTEMARK_UNMATCHED,
    ET_BLOCK_COMMENT_UNCLOSED,
    ET_ESCAPE_SEQUENCE_INVALID,

    // Parser error
    EP_INVALID_CLOSING_BRACKET,
    EP_MISMATCHED_BRACKET,
    EP_INVALID_OPERATOR,
    EP_EXPECTED_PREFIX_OPERATOR,
    EP_EXPECTED_CLOSING_BRACKET,
    EP_EXPECTED_CLOSING_SQUARE_BRACKET,
    EP_TOKEN_UNMATCHED,

};

// Each token contains some error info
typedef struct {

    // false = won't be printed
    char throwable;

    struct {
        // NULL if console
        char* filename;

        unsigned int line, character;
    } fileinfo;

    // TODO: add thread info & name

} errorinfo;

// Throw selected error, print it to stderr
void throw(enum errorformat format, errorinfo *info);