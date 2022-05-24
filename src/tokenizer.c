#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "tokenizer.h"

// TODO: optimize
char next(tokencache *cache) {
    int c = getc(cache->input);
    if(c == '\n') {
        cache->info.line++;
        cache->info.character = 1;
    } else
        cache->info.character++;
    // Return null character if EOF, since \0 is not valid source code anyway
    return c == EOF ? 0 : c;
}

// Reads a new token from cache->input into cache->dst, also updating debug info
// TODO: implement max length
void readtkn(tokencache *cache) {
    token *dst = cache->cur; 

    // retry label to jump back if unnecessary comment was encountered
    retry:

    // content pointer to write text to
    char *con = dst->content;

    // get first character
    char cur = next(cache);

    // remove leading spaces
    while(cur && isspace(cur))
        cur = next(cache);

    // If EOF, return
    if(!cur){
        dst->type = NULLTKN;
        return;
    }

    // TODO: reorder
    if(isdigit(cur)){
        // set type
        dst->type = NUMBER;

        // read number until no alphanumeric or .
        do {
            // append character
            *con++ = cur;
            cur = next(cache);
        } while (isalnum(cur) || cur == '.');

        // push back last character if not space
        if(!isspace(cur)){
            putc(cur, cache->input);
            cache->info.character--;
        }

    } else if(isalpha(cur) || cur == '_') {
        // set type
        dst->type = FIELD;

        // read field until not alphanumeric or _
        do {
            *con++ = cur;
            cur = next(cache);
        } while (isalnum(cur) || cur == '_');

        // push back last character if not space
        if(!isspace(cur)){
            putc(cur, cache->input);
            cache->info.character--;
        }

    } else {

        char quotation = '"';

        // Determine type if unknown
        switch(cur) {
            // Match strings
            case '\'':
                // set to single quotation
                quotation = '\'';
            case '"':
                dst->type = STRING;

                cur = next(cache);
                // ? maybe also disallow newlines
                while(cur != quotation) {

                    // Match escape sequence
                    if(cur == '\\'){
                        // read next character
                        cur = next(cache);
                        // TODO: throw error if quotes not closed
                        if(!cur || cur == quotation)
                            break;

                        switch (cur) {
                            case 't':
                                *con++ = '\t';
                                break;
                            case 'b':
                                *con++ = '\b';
                                break;
                            case 'n':
                                *con++ = '\n';
                                break;
                            case 'r':
                                *con++ = '\r';
                                break;
                            case 'f':
                                *con++ = '\f';
                                break;
                            default:
                                // Nothing is printed if escape sequence is invalid
                                if(cur == '\'' || cur == '"' || cur == '\\')
                                    *con++ = cur;
                                break;
                        }
                    }
                    
                    *con++ = cur;
                    cur = next(cache);
                };

                break;
            
            // Match brackets
            case '(':
            case ')':
            case '{':
            case '}':
            case '[':
            case ']':
                dst->type = BRACKET;
                *con++ = cur;
                break;

            // check for comments
            case '/':
                cur = next(cache);

                // ? might want to write comment to con if necessary

                switch(cur) {
                    case '/':
                        // skip all comment characters
                        do {
                            cur = next(cache);
                        } while(cur && cur != '\n');

                        // discard this token and retry
                        goto retry;
                        break;

                    // handle block comment
                    case '*':
                        // skip all block comment characters
                        unsigned char last;
                        do {
                            last = cur == '*';
                            cur = next(cache);
                        } while(cur && last && cur == '/');

                        // discard this token and retry
                        goto retry;
                        break;
                    
                    case 0:
                        // ignore
                        break;

                    default:
                        // just write the two read signs
                        *con++ = '/';
                        break;
                }
            // Fallthrough

            // Must be any other symbol
            default:
                // set type
                dst->type = SYMBOL;

                // read all following punctuation characters
                while (ispunct(cur)) {
                    *con++ = cur;
                    cur = next(cache);
                };

                // push back last read character if not space
                if(cur && !isspace(cur)){
                    putc(cur, cache->input);
                    cache->info.character--;
                }

                break;
        }
    }

    // append \0 character
    *con = 0;
    return;

}