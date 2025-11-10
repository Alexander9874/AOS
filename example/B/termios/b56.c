// b56.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

void f(int sig) {
    printf("Signal %d received\n", sig);
}

int main() {
    alarm(10);
    signal(SIGINT, f);
    signal(SIGQUIT, f);
    signal(SIGTSTP, f);
    for (;;);
}