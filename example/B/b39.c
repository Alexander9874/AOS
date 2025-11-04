// b39.c
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
    int fd[2];
    signal(SIGPIPE, SIG_IGN);
    pipe(fd);
    close(fd[0]);
    if (write(fd[1], "", 1)==-1)
        perror("write");
    exit(0);
}