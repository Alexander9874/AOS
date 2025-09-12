#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

extern int errno;
extern char ** environ;

int main(int argc, char * argv[], char * envp[]) {
    int i;
    printf ("Before add\n");
    for (i=0; envp[i]; i++)
        printf("%s\n", envp[i]);
    putenv("A=AAA");
    putenv("B=BBB");
    printf("After add\n");
    for (i=0; envp[i]; i++)
        printf("%s\n", envp[i]);
    printf ("After add with environ\n");
    for (i=0; environ[i]; i++)
        printf("%s\n", environ[i]);
    exit (0);
}