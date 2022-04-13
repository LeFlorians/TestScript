/*
* The Tokenizer takes a string and converts it to easy-to-understand tokens
*/

int main() {

    return 0;
}

// Define what a token looks like
typedef struct {
    enum {SCOPE_DECLARATION, FIELD, OPERATOR} type;
    char* content;
    token* parent, leftChild, rightChild;
} token;

/*
* List of operators
* => -> . ~ ! == 
*/

token* tokenize(char* string, int length) {
    int charIndex = 0;
    token currentToken;
    

    while(1) {
        char currentChar = string[currentChar];
        if(currentChar == "\0")
            break;
        
    }

    return &currentToken;
}