#include "runtime.h"

void process(bytecode code, runtime *rt) {
    char byte;

    // Iterate through all bytes of bytecode
    for(char *ptr = code.ptr; ptr < code.ptr + code.size; ptr++) {
        switch(byte & 192) {

            // A value
            case 64:
                // switch by value type
                switch(byte & 48) {
                    // Number
                    case 0:

                        break;

                    // String
                    case 16:

                    // FALLTHROUGH

                    // Field
                    case 32:



                    
                        break;
                }

                break;

            // BLOCK
            case 128:

                break;

            // BLOCK_END
            case 192:

                break;

            // An operation
            case 0:

                break;
        }

    }

}