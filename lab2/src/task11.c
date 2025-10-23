// task11.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "wrapper.h"

const char *const red = "\033[31m";
const char *const reset = "\033[0m";

int main(int argc, char ** argv) {
    
    char command[4096] = "";

    if (argc < 2) {
        _fprintf(stderr, "Usage: %s command args ...\n", argv[0]);
        exit(1);
    }

    if (!_fork()) {
        _printf("%sSon:%s\n", red, reset);
        execvp(argv[1], argv + 1);
        perror(argv[1]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        strcat(command, " ");
    }

    _wait(NULL);
    _printf("%sFather:%s\n", red, reset);
    system(command);

    exit(0);
}
