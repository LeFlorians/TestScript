#include <stdio.h>
#include <stdbool.h>>
#include <unistd.h>

int main(int argc, char** argv){
    int opt;

    while ((opt = getopt(argc, argv, "ilw:")) != -1) {
        switch (opt) {

        default:
            fprintf(stderr, "Usage: %s [-ilw] [file...]\n", argv[0]);
            exit(1);
        }
    }

    if(optind >= argc) {
        // Process stdin

    } else {
        // Process files

    }
}