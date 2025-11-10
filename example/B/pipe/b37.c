// b37.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main() {
    int fd[2], i;
    pipe(fd);
    fcntl(fd[1], F_SETFL, O_NDELAY);
    for(i=0; write(fd[1], "", 1)!=-1; i++)
        ;
    printf("%d\n", i);
    exit(0);
}