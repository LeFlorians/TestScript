#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "tokenizer.h"

int _line = 0, _char;

// Reads the next token from stream
void readtkn(FILE *input, token *dst) {

    // Character variable (ints for non-ASCII  charsets)
    int cur;
    char *con = dst->content;

    /*
        Bitflags:
        0 | Escape next character in string
        1 | 1 if string is "", 0 if ''
        2 | First character matches any of + -
        3 | 1 if Actual token has begun
        4 | unused
        5 | 0=line comment, 1=block comment
        6 | unused
        7 | unused
    */
    char flags = 0;

    // Set type to invalid by default
    dst->type = INVALID;

    while((cur = getc(input)) != EOF) {

        if(cur == '\n'){
            _line++;
            _char = 0;
        }

        _char++;

        if((flags & 8) == 0) {
            if(isspace(cur))
                continue;
            flags |= 8;

            // set debug info
            dst->info.character = _char;
            dst->info.line = _line;
        }

        if(con - dst->content >= 255){
            strcpy(dst->content, "Token exceeded maximum length of 256");
            dst->type = INVALID;
            break;
        }

        // --- Tokenize ---

        // Check if in string
        if(dst->type == STRING) {
            // check if character is escaped
            if((flags & 1) != 0) {
                // Match escape sequence (Java-like)
                con--;
                switch (cur) {
                    case 't':
                        *con = '\t';
                        break;
                    case 'b':
                        *con = '\b';
                        break;
                    case 'n':
                        *con = '\n';
                        break;
                    case 'r':
                        *con = '\r';
                        break;
                    case 'f':
                        *con = '\f';
                        break;
                    default:
                        // Nothing is printed if escape sequence is invalid
                        if(cur == '\'' || cur == '"' || cur == '\\')
                            *con = cur;
                        break;
                }

                // Unset escape flag
                flags &= ~1;
                con++;
                continue;
            } else {
                if( ((flags & 2) != 0 && cur == '"') 
                 || ((flags & 2) == 0 && cur == '\'') ){
                    // Done with string, quit loop
                    break;
                }
                // Set escape flag
                if(cur == '\\'){
                    flags |= 1;
                }
            }

            // append character
            *(con++) = cur;

            continue;
        }

        if(dst->type == COMMENT) {
             
             // If block comment
             if((flags & 32) != 0) {
                // Check if block comment ends 
                if(cur == '/' && con - dst->content >= 1 && *(con - 1) == '*') {
                    con--;
                    break;
                }
             } else if(cur == '\n') {
                // Line comment is over
                break;
             }

            *(con++) = cur;
            continue;
        }

        // Check if type is yet undefined
        if(dst->type == INVALID) {
            
            // String with '
            if(cur == '\'') {
                dst->type = STRING;
                continue;
            }

            // String with "
            if(cur == '"'){
                dst->type = STRING;
                flags |= 2;
                continue;
            }

            if(isalpha(cur))
                dst->type = FIELD;
            else if(isdigit(cur))
                dst->type = NUMBER;
            else if(cur == '(' || cur == ')' || cur == '{' || cur == '}' || cur == '[' || cur == ']'){
                dst->type = BRACKET;
                *(con++) = cur;
                break;
            } 
            else if (ispunct(cur))
                dst->type = SYMBOL;
        
        }

        if(isspace(cur))
            break;
        
        if(dst->type == FIELD && ispunct(cur)) {
            // End field upon any punctuation character
            ungetc(cur, input);
            break;
        }


        if(dst->type == NUMBER) {
            // Check for non-number characters 
            if(!isalnum(cur)){

                if(cur == '.') {
                    if(*(con-1) == '.') {
                        ungetc(cur, input);
                        ungetc('.', input);
                        con--;
                        flags &= ~16; // Unset floating-point flag
                        break;
                    }
                } else if(!((cur == '+' || cur == '-') && *(con-1) == 'e')) {
                    ungetc(cur, input);
                    break;
                }

            }

        }

        // Check for comment
        if(dst->type == SYMBOL){

            // Check for comment in this token
            if(con - dst->content == 1 && dst->content[0] == '/'){
                switch (cur) {
                    case '*':
                        flags |= 32; // set block comment flag
                    case '/':
                        dst->type = COMMENT;
                        con = dst->content; // reset content pointer
                        continue;
                }
            }

            // Check for symbol ending
            if(!ispunct(cur) || cur == '\'' || cur == '"' ||
                cur == '(' || cur == ')' || cur == '{' || cur == '}' || cur == '[' || cur == ']') {
                // End symbol if string or not puncuation character
                ungetc(cur, input);
                break;
            }

            // Check for following comment
            if((cur == '*' || cur == '/') && con - dst->content >= 2 && *(con-1) == '/'){
                // End symbol because of comment
                ungetc(cur, input);
                ungetc('/', input);
                con--;
                break;
            }
        }

        *(con++) = cur;
    }

    // End of stream
    if((flags & 8) == 0) {
        dst->type = NULLTKN;
        dst->content[0] = '\0';
        return;
    }

    // Set error message on invalid token
    if(dst->type == INVALID) {
        strcpy(dst->content, "Encountered invalid token.");
        return;
    }

    // Make sure last character is a 0
    *con = '\0';

    if(dst->type == NUMBER) {
        // TODO: Check if pattern matches

    }

}