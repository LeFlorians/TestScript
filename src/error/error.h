// Enum of all messages
enum errorformat {
    // Tokenizer errors
    ET_QUOTEMARK_UNMATCHED,
    ET_BLOCK_COMMENT_UNCLOSED,
    ET_ESCAPE_SEQUENCE_INVALID,
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