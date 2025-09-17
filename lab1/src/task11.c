#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#include "wraper.h"

int main(int argc, char ** argv) {
    struct stat statbuf;
    int i;
    long size_max = -1;
    int arg_max;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s file ...\n", argv[0]);
        exit(1);
    }
    
    for (i = 1; i < argc; i++) {
        if (stat(argv[i], &statbuf) == 0) {
            if (size_max < statbuf.st_size) {
                size_max = statbuf.st_size;
                arg_max = i;
            }
        }
        else
            perror(argv[i]);
    }
    if (size_max != 0)
        _printf("%s %ld\n", argv[arg_max], size_max);
    exit(0);
}
