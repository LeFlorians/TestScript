#include <stdio.h>
#include <ctype.h>

#include "tokenizer.h"

// Reads the next token from stream
void tokenize(FILE *input, token *dst) {
    // Character variable (ints for non-ASCII  charsets)
    int current;

    /*
        Bitflags:
        0 | If we're currently in a string
        1 | Escape next character in string
        2 | 1 if string is "", 0 if ''
        3 | If we're in a number sequence
        4 | If we're at the right position for a decimal/ocatal/hex declaration
    */
    char flags = 0;

    int content_index = 0;

    while((current = getc(input)) != EOF) {

        if(content_index >= 256){
            perror("Token length exceeded length limit of 255. Halting program.");
            exit(1);
        }

        // --- Tokenize ---

        // Check if in string
        if(flags & 1 != 0) {
            // check if character is escaped
            if(flags & 2 != 0) {
                // Match escape sequence (Java-like)
                switch (current) {
                    case 't':
                        dst->content[content_index++] = '\t';
                        break;
                    case 'b':
                        dst->content[content_index++] = '\b';
                        break;
                    case 'n':
                        dst->content[content_index++] = '\n';
                        break;
                    case 'r':
                        dst->content[content_index++] = '\r';
                        break;
                    case 'f':
                        dst->content[content_index++] = '\f';
                        break;
                    default:
                        // Nothing is printed if escape sequence is invalid
                        if(current == '\'' || current == '"' || current == '\\')
                            dst->content[content_index++] = current;
                        break;
                }

                // Unset escape flag
                flags &= ~2;
                continue;
            } else {
                if((flags & 4 != 0 && current == '"') || (flags & 4 == 0 && current == '\'') ){
                    // Done with string, return
                    break;
                }
                // Set escape flag
                if(current = '\\'){
                    flags |= 2;
                }
            }

            // append character
            dst->content[content_index++] = current;

        } else {
            // Skip spaces
            if(isspace(current))
                continue;

            // Check if number
            if(flags & 8 != 0) {
                if(!isdigit(current) && (current != 'b')) {
                    // push back the last character
                    ungetc(current, input);
                    break;
                }
            }

        }
        






    }


}