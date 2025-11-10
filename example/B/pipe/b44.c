// b44.c
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern int errno;

int main() {
    int fd[2];
    pipe(fd);
    if (fork()) {
        close(fd[1]);
        close(0);
        dup(fd[0]);
        close(fd[0]);
        execlp("wc", "wc", "-l", NULL);
        perror("wc");
        exit(1);
    }
    else {
        close(fd[0]);
        close(1);
        dup(fd[1]);
        close(fd[1]);
        execlp("who", "who", NULL);
        perror("who");
        exit(1);
    }
    exit(0);
}