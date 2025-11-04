// b38.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int main() {
    int fd[2];
    pipe(fd);
    close(fd[0]);
    write(fd[1], "", 1);
    printf("After write\n");
    exit(0);
}