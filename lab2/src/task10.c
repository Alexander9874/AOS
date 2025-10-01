// task10.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "wraper.h"

const char *const red = "\033[31m";
const char *const reset = "\033[0m";

int main(int argc, char ** argv, char ** envp) {

    _printf("%sArguments:%s\n", red, reset);
    for (int i = 0; i < argc; i++)
        _printf("\t%s\n", argv[i]);

    _printf("\n%sEnvironment:%s\n", red, reset);
    for (int i = 0; envp[i]; i++)
        _printf("\t%s\n", envp[i]);

    if (!_fork()) {
        execve(argv[1], argv + 1, NULL);
        perror(argv[1]);
    }

    exit(0);
}
