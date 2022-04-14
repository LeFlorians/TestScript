#include <stdio.h>
#include <stdbool.h>>
#include <unistd.h>

int main(int argc, char** argv){

    // For later: https://www.gnu.org/software/libc/manual/html_node/Streams-and-Threads.html
    FILE *input;

    if(argc >= 2) {
        // open a file stream
        input = fopen(argv[1], "r");
    } else {
        // Just read from standard input
        input = stdin;
    }

}