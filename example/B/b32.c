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
    if ((p = fork())) {
        printf("Parent pid=%d Son pid=%d\n", getpid(), p);
        printf("Setpgrp return %d\n", setpgrp());
        printf("I am father. My pgid=%d sid=%d\n", getpgrp(), getsid(getpid()));
        pause();
    }
    else {
        printf("I am son. My pgid=%d sid=%d\n", getpgrp(), getsid(getpid()));
        setpgrp();
        printf("I am son. My pgid=%d sid=%d\n", getpgrp(), getsid(getpid()));
        fork();
        pause();
    }
}