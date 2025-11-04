// b25.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int i;
void f(){
    printf("%d\n", i);
    exit(0);
}

int main(){
    int fd[2];
    signal(SIGALRM, f);
    alarm(1);
    pipe(fd);
    for(i=0; write(fd[1], "", 1)!=-1; i++)
        ;
}