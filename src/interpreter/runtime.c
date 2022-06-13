#include "runtime.h"

void process(bytecode code) {
    char byte;

    // Iterate through all bytes of bytecode
    while(1) {
        byte = *((char *)pop(code, 1));

        // switch by first two bytes (type)
        switch(byte & 0b11000000) {

            // A value
            case 64:
                // switch by value type
                switch(byte & 0b110000) {
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