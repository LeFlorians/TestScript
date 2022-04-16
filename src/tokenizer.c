#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "tokenizer.h"

// Reads the next token from stream
void tokenize(FILE *input, token *dst) {
    // Character variable (ints for non-ASCII  charsets)
    int cur;
    char *con = dst->content;

    /*
        Bitflags:
        0 | Escape next character in string
        1 | 1 if string is "", 0 if ''
        2 | First character matches any of + -
        3 | 1 if Actual token has begun
        4 | If number contains a dot / is floating-point
        5 | If number started with a sign
    */
    char flags = 0;

    // Set type to invalid by default
    dst->type = INVALID;

    while((cur = getc(input)) != EOF) {

        if(isspace(cur) && (flags & 8) == 0)
            continue;
        flags |= 8;

        if(con - dst->content >= 256){
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
                switch (cur) {
                    case 't':
                        *(con++) = '\t';
                        break;
                    case 'b':
                        *(con++) = '\b';
                        break;
                    case 'n':
                        *(con++) = '\n';
                        break;
                    case 'r':
                        *(con++) = '\r';
                        break;
                    case 'f':
                        *(con++) = '\f';
                        break;
                    default:
                        // Nothing is printed if escape sequence is invalid
                        if(cur == '\'' || cur == '"' || cur == '\\')
                            *(con++) = cur;
                        break;
                }

                // Unset escape flag
                flags &= ~1;
                continue;
            } else {
                if(((flags & 2) != 0 && cur == '"') || ((flags & 2) == 0 && cur == '\'') ){
                    // Done with string, quit loop
                    break;
                }
                // Set escape flag
                if(cur == '\\'){
                    flags |= 2;
                }
            }

            // append character
            *(con++) = cur;

        } else {

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

                if(con - dst->content == 0) {
                    if(cur == '+' || cur == '-') {
                        flags |= 32;
                        *(con++) = cur;
                    }
                }


                if(isalpha(cur) || ispunct(cur))
                    dst->type = FIELD;
                else if(isdigit(cur))
                    dst->type = NUMBER;
                else if(cur == '(' || cur == ')' || cur == '{' || cur == '}' || cur == '[' || cur == ']'){
                    dst->type = BRACKET;
                    *(con++) = cur;
                    break;
                }
            
            }

            if(isspace(cur))
                break;

            if(dst->type == NUMBER) {
                int loc = con - dst->content;
                // Check for non-digit characters in number
                if(!isdigit(cur) 
                    && !( (((flags & 32) != 0 && (loc == 2)) || ((flags & 32) == 0 || (loc == 1)) ) 
                    && (cur == 'b' || cur == 'B' || cur == 'x' || cur == 'X' || cur == 'o' || cur == 'O') ) ){
                    
                    // Invalid character found
                    ungetc(cur, input); // push back character
                    break;

                }

            }

            *(con++) = cur;

            if(cur == '.')
                flags |= 16; // set floating point flag

        }
    }

    // Set error message on invalid token
    if((flags & 8) != 0 && dst->type == INVALID) {
        strcpy(dst->content, "Reached end of stream / Uncategorizable token");
        return;
    }

    // Make sure last character is a 0
    *con = '\0';

    // Check if it's really a number
    if(dst->type == NUMBER) {
        // Reset con
        con = dst->content;
        if((flags & 16) == 0) {

            // parse base
            int base = 10;

            if(dst->content[0] == '0'){
                char base_char = dst->content[1];
                if(base_char == 'x' || base_char == 'X')
                    base = 16;
                else if(base_char == 'b' || base_char == 'B')
                    base = 2; 
                else if(base_char == 'o' || base_char == 'O')
                    base = 8;

                if(base != 10)
                    con += 2; // skip base charactesr if valid
            }

            dst->type = INTEGER;

            char* end;
            dst->num.integer = strtoll(con, &end, base);
            if(*end != '\0' || *con == '\0') {
                // Invalid integer
                strcpy(dst->content, "Invalid integer number format");
                dst->type = INVALID;
                return;
            }

        } else {
            char* end;
            dst->num.number = strtod(con, &end);
            if(*end != '\0' || *con == '\0') {
                // Invalid floating-point
                strcpy(dst->content, "Invalid floating-point number format");
                dst->type = INVALID;
                return;
            }
        }
    }


}