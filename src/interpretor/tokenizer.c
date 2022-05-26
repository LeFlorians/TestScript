#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "tokenizer.h"

// local filepos struct

// TODO: optimize
char next(cache *cache) {
    // enforce max length
    if(cache->tokenpos == MAX_CONTENT) {
        throw(ET_TOKEN_LENGTH_EXCEEDED, &cache->info);
        return 0;
    }

    cache->tokenpos++;

    int c = getc(cache->input);
    if(c == '\n') {
        cache->info.fileinfo.line++;
        cache->info.fileinfo.character = 1;
    } else
        cache->info.fileinfo.character++;
    // Return null character if EOF, since \0 is not valid source code anyway
    return c == EOF ? 0 : c;
}

// Reads a new token from cache->input into cache->dst, also updating debug info
void readtkn(cache *cache) {
    token *dst = cache->cur; 

    // retry label to jump back if unnecessary comment was encountered
    retry:

    // reset token position
    cache->tokenpos = 0;

    // content pointer to write text to
    char *con = dst->content;

    // get first character
    char cur = next(cache);

    // remove leading spaces
    while(cur && isspace(cur))
        cur = next(cache);

    // If EOF, return
    if(!cur){
        error:

        dst->content[0] = '\0';
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
            ungetc(cur, cache->input);
            cache->info.fileinfo.character--;
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
            ungetc(cur, cache->input);
            cache->info.fileinfo.character--;
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
                while(cur != quotation) {

                    // Match escape sequence
                    if(cur == '\\'){
                        // read next character
                        cur = next(cache);
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
                                // Error is thrown if escape sequence is invalid
                                if(cur == '\'' || cur == '"' || cur == '\\')
                                    *con++ = cur;
                                else {
                                    throw(ET_ESCAPE_SEQUENCE_INVALID, &cache->info);
                                    goto error;
                                }
                                break;
                        }

                        cur = next(cache);  
                        continue;
                    
                    } else if(!cur || cur == '\n') {
                        // throw error because quotation marks not closed (on EOF or newline)
                        throw(ET_QUOTEMARK_UNMATCHED, &cache->info);
                        goto error;
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
                        cur = next(cache);
                        
                        // skip all block comment characters
                        unsigned char last = 0;
                        while(cur && !(last && cur == '/')) {
                            last = cur == '*';
                            cur = next(cache);
                        };

                        if(!cur) {
                            throw(ET_BLOCK_COMMENT_UNCLOSED, &cache->info);
                            goto error;
                        }

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
                    ungetc(cur, cache->input);
                    cache->info.fileinfo.character--;
                }

                break;
        }
    }

    // append \0 character
    *con = 0;
    return;

}