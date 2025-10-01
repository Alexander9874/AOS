// task1.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "wraper.h"

extern char ** environ;

const char *const red = "\033[31m";
const char *const reset = "\033[0m";

int main(int argc, char * argv[], char * envp[]) {
    int i;

    if (argc != 1) {
        _fprintf(stderr, "Usage: %s\n",
                 argv[0]);
        exit(1);
    }

    _printf("%sBefore add%s\n", red, reset);
    for (i = 0; envp[i]; i++)
        _printf("%s\n", envp[i]);

    _putenv("A=AAA");
    _putenv("B=BBB");
    
    _printf("%sAfter add%s\n", red, reset);
    for (i = 0; envp[i]; i++)
        _printf("%s\n", envp[i]);
    
    _printf("%sAfter add with environ%s\n", red, reset);
    for (i = 0; environ[i]; i++)
        _printf("%s\n", environ[i]);
    
    exit(0);
}
