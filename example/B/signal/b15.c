//b15.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

extern int errno;

void f(int sig) {
    printf("Signal received\n");
}

int main() {
    struct sigaction act;
    act.sa_handler = f;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &act, NULL);
    for (;;);
}
