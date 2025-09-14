// a04.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

extern int errno;
extern char ** environ;

const char *const red = "\033[31m";
const char *const reset = "\033[0m";

int main(int argc, char * argv[], char * envp[]) {
    int i;
    printf("%sBefore add%s\n", red, reset);
    for (i = 0; envp[i]; i++)
        printf("%s\n", envp[i]);
    putenv("A=AAA");
    putenv("B=BBB");
    printf("%sAfter add%s\n", red, reset);
    for (i = 0; envp[i]; i++)
        printf("%s\n", envp[i]);
    printf("%sAfter add with environ%s\n", red, reset);
    for (i = 0; environ[i]; i++)
        printf("%s\n", environ[i]);
    exit(0);
}
