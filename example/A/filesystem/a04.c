// a04.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

extern int errno;
extern char ** environ;

#define RED "\033[31m"
#define RESET "\033[0m"

int main(int argc, char * argv[], char * envp[]) {
    int i;
    printf(RED "Before add\n" RESET);
    for (i = 0; envp[i]; i++)
        printf("%s\n", envp[i]);
    putenv("A=AAA");
    putenv("B=BBB");
    printf(RED "After add\n" RESET);
    for (i = 0; envp[i]; i++)
        printf("%s\n", envp[i]);
    printf(RED "After add with environ\n" RESET);
    for (i = 0; environ[i]; i++)
        printf("%s\n", environ[i]);
    exit(0);
}
