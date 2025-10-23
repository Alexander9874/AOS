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
    int p;
    // if (p = fork()) {
    if ((p = fork())) {
        printf("Parent pid=%d Son pid=%d\n", getpid(), p);
        pause();
    }
    else {
        pause();
    }
}